This is what our assignment is about:

Simulates a user walking around a circular path w/ one thread per location.

A mutex protects the user's current index (loc) and a stop flag (over).

EAch thread waits on its own condition variable until the user arrives at the index, then computes next index, prints, decrements value, updates loc, and signals next thread.

If value = 0, then it's over and lets each thread exit. Then we report final location

We avoid races and deadlocks
