# Design Document for Project 1: Threads

#### Author:

- 唐千栋 [11612730]()

## Task 1: Efficient Alarm Clock

### Data Structure and Functions

In `threads/threads.h`, we add

-  a variable `wake_time` in `struct thread` to record when the thread would wake up.

  ```c
  /* a variable to record when the thread would wake up */
  int64_t wake_time;
  ```

In `threads/threads.c`, we add 

- a global static list `sleep_list` to record the sleeping thread.

  ```c
  /* List of processes in sleep */
  static struct list sleep_list;
  ```

- a function `check_sleep()` to check whether sleeping thread should wake up.

### Algorithms

In `void timer_sleep(int64_t ticks)`

1. `wake_time` would be set to current time_tick + ticks.
2. Disable the interrupt.
3. Put current thread into `sleep_list`
4. Block the thread
5. Undo the interrupt level

In `void check_sleep(void)`

1. Traverse all the sleeping threads in `sleep_list`
2. If thread `wake_time` >= current timer ticks,  ignore it.
3. else remove  the thread from the sleeping list, and unblock this thread.

In `void thread_ticks(void)`

- Since this function would be called by timer interrupt handler at each timer tick, we could call `check_sleep()` inside it.

### Synchronization

The linked list `sleep_list` could be accessed by two threads at the same time. Through disable interrupts when inserting threads into `sleep_list`, multiple threads could not access it at the same time. Interrupts are also been disabled in `thread_block()` and `thread_unblock()` to prevent this function being called in 2 threads at the same time.

### Rationale

Using a list `sleep_list` to store sleeping threads is a straight forward design. It is also easy to implement. Furthermore, inserting a thread into `sleep_list` takes O(1) time. Traverse `sleep_list` takes O(n) times.



## Task 2: Priority Scheduler

### Data Structures and Functions

In `threads/threads.h`, we 

- add to `struct thread`
  - a list `locks` to record descending ordered by the thread priority
  - a variable `real_priority` to record the real priority without donation.

In `threads/synch.c`, we

- add to `struct lock`
  - a flag `is_donated` to record whether the lock is donated to its holder
- add to `struct semapore`
  - a variable `priority` to record the priority value

### Algorithms

In `void lock_acquire(struct lock *lock)`

- Disable interrupts
- Compare the `lock` priority with current thread's priority
- If `lock` priority > current priority
  - call `sema_down` until SEMA's value become positive and `lock` release
- else 
  - set `lock` priority to current priority
  - call `sema_down` until SEMA's value become positive and lock release
- undo interrupt level

### Synchronization

Since the lock and it's donor would change lock's priority at the same time, it would cause a problem. We disable the interrupts to  avoid this happen. 

### Rationale



## Task 3: Multi-level Feedback Queue



## Questions



