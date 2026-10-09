			+--------------------+
			|        CS 318      |
			| PROJECT 1: THREADS |
			|   DESIGN DOCUMENT  |
			+--------------------+
				   
---- GROUP ----

>> Fill in the names and email addresses of your group members.

Chamath Jayasinghe chamath.24@cse.mrt.ac.lk

---- PRELIMINARIES ----

>> If you have any preliminary comments on your submission, notes for the
>> TAs, or extra credit, please give them here.

>> Please cite any offline or online sources you consulted while
>> preparing your submission, other than the Pintos documentation, course
>> text, lecture notes, and course staff.

No additional sources were consulted.

			     ALARM CLOCK
			     ===========

---- DATA STRUCTURES ----

>> A1: Copy here the declaration of each new or changed `struct' or
>> `struct' member, global or static variable, `typedef', or
>> enumeration.  Identify the purpose of each in 25 words or less.

	int64_t wakeup_tick;                /* Tick at which the thread wakes. */

	static struct list sleeping_list;

`wakeup_tick` stores the absolute timer tick at which a blocked thread
should wake.  `sleeping_list` stores sleeping threads in wakeup-tick order.

---- ALGORITHMS ----

>> A2: Briefly describe what happens in a call to timer_sleep(),
>> including the effects of the timer interrupt handler.

For a positive duration, timer_sleep() disables interrupts, computes the
thread's absolute wakeup tick, inserts the thread into sleeping_list in
sorted order, and blocks it.  For zero or negative durations it returns
immediately.  Each timer interrupt increments ticks and removes every
sleeping thread whose wakeup_tick has arrived, moving those threads to the
ready list with thread_unblock().

>> A3: What steps are taken to minimize the amount of time spent in
>> the timer interrupt handler?

Sleeping threads are kept ordered by wakeup_tick.  The interrupt handler
examines only the front of the list and stops at the first thread whose
wakeup time has not arrived.  Therefore it does not scan all sleeping
threads on every tick; it removes only threads that are ready to wake.

---- SYNCHRONIZATION ----

>> A4: How are race conditions avoided when multiple threads call
>> timer_sleep() simultaneously?

timer_sleep() disables interrupts before reading the current tick, setting
the wakeup_tick, and inserting the thread.  Thus only one thread at a time
can modify sleeping_list, and each thread is inserted with a consistent
absolute wakeup time.

>> A5: How are race conditions avoided when a timer interrupt occurs
>> during a call to timer_sleep()?

Interrupts are disabled before the thread is inserted and remain disabled
through thread_block().  Consequently, the timer interrupt cannot run
between the wakeup check and blocking the thread.  The interrupt can only
unblock the thread after it has been completely inserted into sleeping_list.

---- RATIONALE ----

>> A6: Why did you choose this design?  In what ways is it superior to
>> another design you considered?

This design avoids wasting CPU time: a sleeping thread is blocked instead
of repeatedly yielding while waiting.  The ordered list makes wakeup
processing efficient and uses the existing thread list element, so no
dynamic allocation is required.  A busy-wait or repeated-yield design
would consume scheduler time and perform unnecessary context switches.

			 PRIORITY SCHEDULING
			 ===================

---- DATA STRUCTURES ----

>> B1: Copy here the declaration of each new or changed `struct' or
>> `struct' member, global or static variable, `typedef', or
>> enumeration.  Identify the purpose of each in 25 words or less.

No priority-scheduling data structures have been implemented yet.  The
only changed thread member is `wakeup_tick`, documented in A1; it belongs
to the alarm-clock implementation, not priority donation.

>> B2: Explain the data structure used to track priority donation.
>> Use ASCII art to diagram a nested donation.  (Alternately, submit a
>> .png file.)

Priority donation is not implemented in the current code.  There is no
donation list, lock ownership metadata beyond lock->holder, or nested
donation data structure to describe.

The intended relationship would look like this:

	H (high priority) --waits for--> lock B --held by--> M
											 M --waits for--> lock A
																  A --held by--> L

	H priority -> M effective priority -> L effective priority

---- ALGORITHMS ----

>> B3: How do you ensure that the highest priority thread waiting for
>> a lock, semaphore, or condition variable wakes up first?

The current implementation does not ensure this.  Semaphore and
condition-variable waiters are added and removed in FIFO order, and the
ready list is also FIFO.  Priority ordering would require ordered waiter
lists and selecting the highest-priority ready thread.

>> B4: Describe the sequence of events when a call to lock_acquire()
>> causes a priority donation.  How is nested donation handled?

Priority donation is not implemented.  lock_acquire() currently waits on
the lock's semaphore and records the holder after acquiring it; it does not
raise the holder's priority or propagate a donation through another lock.

>> B5: Describe the sequence of events when lock_release() is called
>> on a lock that a higher-priority thread is waiting for.

The current lock_release() clears lock->holder and calls sema_up().  FIFO
selection wakes the first semaphore waiter, regardless of priority.  No
priority donation is removed because donation is not implemented.

---- SYNCHRONIZATION ----

>> B6: Describe a potential race in thread_set_priority() and explain
>> how your implementation avoids it.  Can you use a lock to avoid
>> this race?

The current thread_set_priority() only assigns the new value to the
current thread and does not disable interrupts or reschedule.  Since the
current implementation has no priority-based scheduler or donation state,
there is no implemented donation race to avoid.  A future implementation
should update base and effective priority atomically with interrupts
disabled; a normal lock is not appropriate in interrupt context and could
itself block.

---- RATIONALE ----

>> B7: Why did you choose this design?  In what ways is it superior to
>> another design you considered?

No priority-donation design has been implemented yet, so there is no
implementation-specific design choice to compare.  An appropriate future
design should keep a thread's base priority separately from its effective
priority and track donations associated with held locks, allowing all
donations for a released lock to be removed safely.

			  ADVANCED SCHEDULER
			  ==================

---- DATA STRUCTURES ----

>> C1: Copy here the declaration of each new or changed `struct' or
>> `struct' member, global or static variable, `typedef', or
>> enumeration.  Identify the purpose of each in 25 words or less.

MLFQS and fixed-point scheduler data structures have not been implemented.
The `wakeup_tick` member and `sleeping_list` global are alarm-clock data,
not advanced-scheduler data.

---- ALGORITHMS ----

>> C2: Suppose threads A, B, and C have nice values 0, 1, and 2.  Each
>> has a recent_cpu value of 0.  Fill in the table below showing the
>> scheduling decision and the priority and recent_cpu values for each
>> thread after each given number of timer ticks:

timer  recent_cpu    priority   thread
ticks   A   B   C   A   B   C   to run
-----  --  --  --  --  --  --   ------
 0
 4
 8
12
16
20
24
28
32
36

This table is not applicable to the current implementation.  MLFQS is not
implemented: thread_set_nice(), thread_get_nice(),
thread_get_recent_cpu(), and thread_get_load_avg() are stubs, and the
ready-list scheduler does not calculate MLFQS priorities.

>> C3: Did any ambiguities in the scheduler specification make values
>> in the table uncertain?  If so, what rule did you use to resolve
>> them?  Does this match the behavior of your scheduler?

The table cannot be evaluated because the advanced scheduler is not
implemented.  No ambiguity rule was needed for the current code.

>> C4: How is the way you divided the cost of scheduling between code
>> inside and outside interrupt context likely to affect performance?

The current scheduler performs only basic round-robin accounting in the
timer interrupt and does not implement MLFQS calculations.  A future
implementation should keep per-tick accounting small and perform the more
expensive periodic load-average and recent_cpu updates only at their
required intervals.  This limits interrupt latency while keeping scheduler
decisions current.

---- RATIONALE ----

>> C5: Briefly critique your design, pointing out advantages and
>> disadvantages in your design choices.  If you were to have extra
>> time to work on this part of the project, how might you choose to
>> refine or improve your design?

The implemented alarm-clock design is simple, avoids busy waiting, and
uses an ordered list without dynamic allocation.  Its main limitation is
that priority scheduling, donation, and MLFQS are not implemented in the
current repository.  With more time, I would implement priority-ordered
scheduling and donation, then add and test the advanced scheduler.

>> C6: The assignment explains arithmetic for fixed-point math in
>> detail, but it leaves it open to you to implement it.  Why did you
>> decide to implement it the way you did?  If you created an
>> abstraction layer for fixed-point math, that is, an abstract data
>> type and/or a set of functions or macros to manipulate fixed-point
>> numbers, why did you do so?  If not, why not?

Fixed-point arithmetic has not yet been implemented, so no representation
or arithmetic decision has been made.  I would use an explicit abstraction
with conversion, addition, multiplication, and division helpers to keep
scaling and rounding consistent throughout the scheduler.

			   SURVEY QUESTIONS
			   ================

Answering these questions is optional, but it will help us improve the
course in future quarters.  Feel free to tell us anything you
want--these questions are just to spur your thoughts.  You may also
choose to respond anonymously in the course evaluations at the end of
the quarter.

>> In your opinion, was this assignment, or any one of the three problems
>> in it, too easy or too hard?  Did it take too long or too little time?

The alarm-clock portion was manageable.  The priority and advanced
scheduler portions require substantially more coordination between data
structures, synchronization, and scheduling decisions.

>> Did you find that working on a particular part of the assignment gave
>> you greater insight into some aspect of OS design?

Yes.  The alarm clock demonstrated why interrupt masking is needed to make
blocking and wakeup atomic, and why blocking is preferable to busy waiting.

>> Is there some particular fact or hint we should give students in
>> future quarters to help them solve the problems?  Conversely, did you
>> find any of our guidance to be misleading?

A small example showing the exact ordering of interrupt disabling,
insertion into a wait list, and thread_block() would be helpful.

>> Do you have any suggestions for the TAs to more effectively assist
>> students, either for future quarters or the remaining projects?

Short traces showing the expected state of the ready list and blocked
lists during representative tests would make debugging easier.

>> Any other comments?

The project gives useful practice with the interaction between interrupts,
thread state, and scheduler data structures.