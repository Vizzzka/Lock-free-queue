#include <iostream>
#include <atomic>
#include <chrono>
#include <thread>
#include <functional>
#include <queue>
#include <mutex>
#include <condition_variable>


template<typename T>
class threadsafe_queue
{
private:
	mutable std::mutex mut;
	std::queue<T> data_queue;
	std::condition_variable data_cond;
public:
	threadsafe_queue()
	{}

	void push(T&& new_value) // <== викликатиметься для temporary -- mq.push(get_val()); та у випадку mq.push(move(v))
	{
		std::lock_guard<std::mutex> lk(mut);
		data_queue.push(std::move(new_value));
		data_cond.notify_one();
	}

	void wait_and_pop(T& value)
	{
		std::unique_lock<std::mutex> lk(mut);
		data_cond.wait(lk,[this]{return !data_queue.empty();});
		value=std::move(data_queue.front());
		data_queue.pop();
	}

	void wait_and_pop_pair(std::pair<T, T>& pair)
	{
		std::unique_lock<std::mutex> lk(mut);
		data_cond.wait(lk,[this]{return !(data_queue.size() < 2);});
		pair.first=std::move(data_queue.front());
		data_queue.pop();
		if (pair.first.size() == 0) {
			pair.second = pair.first;
			return;
		}
		pair.second=std::move(data_queue.front());
		data_queue.pop();
		if (pair.second.size() == 0) {
			data_queue.push(pair.first);
			pair.first = pair.second;
		}
	}

	std::shared_ptr<T> wait_and_pop()
	{
		std::unique_lock<std::mutex> lk(mut);
		data_cond.wait(lk,[this]{return !data_queue.empty();});
		std::shared_ptr<T> res(
				std::make_shared<T>(std::move(data_queue.front())));
		data_queue.pop();
		return res;
	}

	bool try_pop(T& value)
	{
		std::lock_guard<std::mutex> lk(mut);
		if(data_queue.empty())
			return false;
		value=std::move(data_queue.front());
		data_queue.pop();
		return true;
	}

	std::shared_ptr<T> try_pop()
	{
		std::lock_guard<std::mutex> lk(mut);
		if(data_queue.empty())
			return std::shared_ptr<T>();
		std::shared_ptr<T> res(
				std::make_shared<T>(std::move(data_queue.front())));
		data_queue.pop();
		return res;
	}

	bool empty() const
	{
		std::lock_guard<std::mutex> lk(mut);
		return data_queue.empty();
	}

	int size()
	{
		std::lock_guard<std::mutex> lk(mut);
		return data_queue.size();
	}

	bool full()
	{
		std::lock_guard<std::mutex> lk(mut);
		return data_queue.size() > 100;
	}
};

// time calculating
inline std::chrono::high_resolution_clock::time_point get_current_time_fenced()
{
	std::atomic_thread_fence(std::memory_order_seq_cst);
	auto res_time = std::chrono::high_resolution_clock::now();
	std::atomic_thread_fence(std::memory_order_seq_cst);
	return res_time;
}

template<class D>
inline long long to_us(const D& d)
{
	return std::chrono::duration_cast<std::chrono::microseconds>(d).count();
}

unsigned const max_hazard_pointers=100;
struct hazard_pointer
{
	std::atomic<std::thread::id> id;
	std::atomic<void*> pointer;
};
hazard_pointer hazard_pointers[max_hazard_pointers];
class hp_owner
{
	hazard_pointer* hp;

public:
	hp_owner(hp_owner const&)=delete;
	hp_owner operator=(hp_owner const&)=delete;
	hp_owner():
			hp(nullptr)
	{
		for(unsigned i=0;i<max_hazard_pointers;++i)
		{
			std::thread::id old_id;
			if(hazard_pointers[i].id.compare_exchange_strong(
					old_id,std::this_thread::get_id()))
			{
				hp=&hazard_pointers[i];
				break;
			}
		}
		if(!hp)
		{
			throw std::runtime_error("No hazard pointers available");
		}
	}
	std::atomic<void*>& get_pointer()
	{
		return hp->pointer;
	}
	~hp_owner()
	{
		hp->pointer.store(nullptr);
		hp->id.store(std::thread::id());
	}
};
std::atomic<void*>& get_hazard_pointer_for_current_thread()
{
	thread_local static hp_owner hazard;
	return hazard.get_pointer();
}

bool outstanding_hazard_pointers_for(void* p)
{
	for(unsigned i=0;i<max_hazard_pointers;++i)
	{
		if(hazard_pointers[i].pointer.load()==p)
		{
			return true;
		}
	}
	return false;
}

template<typename T>
void do_delete(void* p)
{
	delete static_cast<T*>(p);
}
struct data_to_reclaim
{
	void* data;
	std::function<void(void*)> deleter;
	data_to_reclaim* next;
	template<typename T>
	data_to_reclaim(T* p):
			data(p),
			deleter(&do_delete<T>),
			next(0)
	{}
	~data_to_reclaim()
	{
		deleter(data);
	}
};
std::atomic<data_to_reclaim*> nodes_to_reclaim;
void add_to_reclaim_list(data_to_reclaim* node)
{
	node->next=nodes_to_reclaim.load();
	while(!nodes_to_reclaim.compare_exchange_weak(node->next,node));
}
template<typename T>
void reclaim_later(T* data)
{
	add_to_reclaim_list(new data_to_reclaim(data));
}
void delete_nodes_with_no_hazards()
{
	data_to_reclaim* current=nodes_to_reclaim.exchange(nullptr);
	while(current)
	{
		data_to_reclaim* const next=current->next;
		if(!outstanding_hazard_pointers_for(current->data))
		{
			delete current;
		}
		else
		{
			add_to_reclaim_list(current);
		}
		current=next;
	}
}
template<typename T>
class lock_free_queue {
private:
	struct node {
		T data;
		std::atomic<node *> next;

		node(): next(nullptr) {}

		node(T const &data_) :
				data(data_), next(nullptr) {}
	public: void print() {
			std::cout << data << "\n";
		}
	};

	std::atomic<node *> head;
	std::atomic<node *> tail;
public:
	lock_free_queue()
	{
		node * p = new node();
		head.store( p, std::memory_order_release );
		tail.store( p, std::memory_order_release );
	}
	void push(T const &data) {
		std::atomic<void *> &hp = get_hazard_pointer_for_current_thread();
		node *old_tail = tail.load(std::memory_order_relaxed);
		node *temp;
		node * pNew = new node(data);
		while(true) {
			// put hazard pointer
			do {
				temp = old_tail;
				hp.store(old_tail);
				old_tail = tail.load();
			} while (tail != temp);

			if (old_tail != tail.load(std::memory_order_acquire)) continue;  // check is tail still consistent
			node * next = (old_tail->next).load(std::memory_order_acquire);
			if (old_tail != tail) continue; // check is tail still consistent

			if (next != nullptr) {
				// we want to push tail
				tail.compare_exchange_weak(
						old_tail, next, std::memory_order_release);
				continue;
			}

			node * n = nullptr;
			// if tail next element is nullptr then we can add new node to old ptr
			if (old_tail->next.compare_exchange_strong(
					n, pNew, std::memory_order_release))
				break;

		}
		// if tail was not changed before then we can add node to actuall tail
		tail.compare_exchange_strong(old_tail, pNew, std::memory_order_acq_rel );
		hp.store(nullptr); // free hazard pointer
	}

	bool pop() {
		std::atomic<void *> &hp0 = get_hazard_pointer_for_current_thread();
		std::atomic<void *> &hp1 = get_hazard_pointer_for_current_thread();
		node *old_head = head.load();
		while (true) {
		  node *temp;
		  do {
			temp = old_head;
			hp0.store(old_head);
			old_head = head.load();
		  } while (old_head != temp);


		  if (old_head != head.load(std::memory_order_acquire)) continue;
		  node * next = old_head->next.load(std::memory_order_acquire);
		  node * old_tail = tail.load(std::memory_order_relaxed);

		  do {
			temp = next;
			hp1.store(next);
			old_head = head.load();
		  } while (old_head->next != temp);

		  if (old_head != head.load(std::memory_order_relaxed))
			continue;
		  if (next == nullptr) {
			// Очередь пуста
			hp0.store(nullptr);
			return false;
		  }

		if (head == tail) {
			// Помогаем методу enqueue – продвигаем m_Tail
			tail.compare_exchange_strong( old_tail, next,
											std::memory_order_release);
			continue;
		  }
		  if ( head.compare_exchange_strong(old_head, next,
											  std::memory_order_release)) {

			if (outstanding_hazard_pointers_for(old_head)) {
			  reclaim_later(old_head);
			} else {
			  delete old_head;
			}
			delete_nodes_with_no_hazards();
			break;
		  }

		}
		// (*head).print();
		return true;

	}
};

lock_free_queue<int> s{};

void thread_function() {
	for (int i = 0; i < 10000000; i++)
		s.push(i);
	for (int i = 0; i < 1000; i++)
		s.pop();
}

void work() {
	for (int i = 0; i < 5; i++) {
		thread_function();
	}
}

void work_parallel() {
	std::thread first (thread_function);
	std::thread second (thread_function);
	first.join();
	second.join();

}

int main() {

	auto before = get_current_time_fenced();
	std::queue<int> q;
	for (int i = 0; i < 4; i++) {
		for (int i = 0; i < 10000000; i++)
			q.push(i);
		for (int i = 0; i < 1000; i++)
			q.pop();
	}
	auto work_q_time = get_current_time_fenced() - before;

	before = get_current_time_fenced();
	work_parallel();
	auto work_parall_time = get_current_time_fenced() - before;

	before = get_current_time_fenced();
	threadsafe_queue<int> sq;
	for (int i = 0; i < 2; i++) {
		for (int i = 0; i < 10000000; i++)
			sq.push(std::move(i));
		for (int i = 0; i < 1000; i++)
			sq.wait_and_pop();
	}
	auto work_blocking = get_current_time_fenced() - before;
	std::cout << "With queue: " << to_us(work_q_time) << "\n";
	std::cout << "With blocking queue: " << to_us(work_blocking) << "\n";
	std::cout << "With lock-free queue: " << to_us(work_parall_time) << "\n";

	return 0;
}