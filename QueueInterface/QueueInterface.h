//
// Created by usach on 14.06.2020.
//

#ifndef ACSPROJECT_QUEUEINTERFACE_H
#define ACSPROJECT_QUEUEINTERFACE_H

#include <string>
#include <mutex>
#include <condition_variable>


template <class T>
class QueueInterface{
public:

	virtual void push(const T &val) = 0;
	virtual T pop() = 0;

	virtual bool empty() const = 0;
	virtual size_t size() const = 0;

	virtual ~QueueInterface() = default;
};

#endif //ACSPROJECT_QUEUEINTERFACE_H
