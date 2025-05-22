2. Code description


1. “Why This Works" explanation: 

We use two threads (Plyer and checker) that coordinate using a shared 'gmn_t' struct protected via a mutex and 2 conditional variables. Thread P does binary search from 1 to Max val and halfs the search space each time and signals C to make a guess. Thread c (chckcer) waits for that signal, compares the guess to the secret using gmn_check(), writes back a result code and message, then signals P to continue. We use this lock and wait pattern so P and C never race or interact with each others data. Binary search runs in o (logn) steps and our synch enforced crrect ordering and exact message formatting. So we always find the secret number efficiently and prints exactly the required output.





