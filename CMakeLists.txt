cmake_minimum_required(VERSION 3.14)
project(ACSProject)

set(CMAKE_CXX_STANDARD 17)


add_executable(ACSProject main.cpp QueueInterface/QueueInterface.h BlockingQueue/BlockingQueue.h LockFreeQueue/LockFreeQueue.h HazardPointers/HazardPointers.h tests.cpp)