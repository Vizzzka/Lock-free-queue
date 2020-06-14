//
// Created by usach on 14.06.2020.
//

#ifndef ACSPROJECT_LOCKFREEQUEUE_H
#define ACSPROJECT_LOCKFREEQUEUE_H

#include "../QueueInterface/QueueInterface.h"
#include <atomic>
#include "../HazardPointers/HazardPointers.h"
#include <iostream>

template<typename T>
class lock_free_queue : public QueueInterface<T>{
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


#endif //ACSPROJECT_LOCKFREEQUEUE_H
