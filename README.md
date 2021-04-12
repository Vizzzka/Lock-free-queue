# Lock-free-queue
C++ library 

<p>The aim of the project is to implement a **lock-free queue** based on the [Michael-Scott algorithm  algorithm](https://duckduckgo.com ).
This is a linked-list-based non-blocking unbounded queue.
Blocking queues allow a slower process to prevent a faster process from completing an operation on a shared structure.
A formal definition is a non-lockable object that ensures that a thread completes operations on an object in a finite number of steps,
regardless of how other threads work (even if they fail). </p>

Valgrind used to view all possible data races and locks, the result is in file valgrind.txt .
