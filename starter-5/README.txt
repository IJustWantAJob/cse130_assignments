2. Code description

1. “Why This Works" explanation: 


I made sure to use fine-grained instead of coarse locks-

We first lock t->lock long enough to check for an empty tree and set root once. After that all synchronization is handled by per-node locks.

Each TNode has its own lock protecting left and right pointers. During traversal we always lock(child) before unlock(parent). This prevents another thread from stealing or deleting the pointer we're about to follow.

If we detect val == curr->val we relase the lock on curr, destroy the new node, and return so no duplicates get inserted.

Since PrintInOrder and destroyTree run onlyt after all worker threads have joined they use simple traversal without any locking.
Since we only handle two locks at a time, we can perform concurrency (threads inserting itno diff parts of tree never block each other).


