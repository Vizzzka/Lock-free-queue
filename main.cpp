#include <iostream>
#include <atomic>
#include <chrono>
#include <thread>
#include <functional>
#include <queue>
#include <mutex>
#include <condition_variable>



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