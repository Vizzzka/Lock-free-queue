//
// Created by usach on 14.06.2020.
//

#ifndef ACSPROJECT_BLOCKINGQUEUE_H
#define ACSPROJECT_BLOCKINGQUEUE_H


#include <queue>
#include "../QueueInterface/QueueInterface.h"


template <class T>
class BlockingQueue final: public QueueInterface<T> {
private:
	mutable std::mutex mut;
	std::queue<T> data_queue;
	std::condition_variable data_cond;
public:
	BlockingQueue() {}

	void push(const T& new_value) override
	{
		std::lock_guard<std::mutex> lk(mut);
		data_queue.push(std::move(new_value));
		data_cond.notify_one();
	}



	T pop() override
	{
		std::unique_lock<std::mutex> lk(mut);
		data_cond.wait(lk,[this]{return !data_queue.empty();});
		T res = std::move(data_queue.front());
		data_queue.pop();
		return res;
	}

	bool pop(T& value) override
	{
		std::lock_guard<std::mutex> lk(mut);
		if(data_queue.empty())
			return false;
		value=std::move(data_queue.front());
		data_queue.pop();
		return true;
	}

	T try_pop()
	{
		std::lock_guard<std::mutex> lk(mut);
		if(data_queue.empty())
			return std::shared_ptr<T>();
		T res = std::move(data_queue.front());
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

	virtual ~BlockingQueue() = default;
};

#endif //ACSPROJECT_BLOCKINGQUEUE_H
