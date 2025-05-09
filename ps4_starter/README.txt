2. Code description

main.c
Parses 3 line command line arguments (two matrix filenames and number of worker processes)
-Loads matrices A and B with readMatrix()
-Creates a POSIX shared-memory object using shm_open, ftruncate, and nmap
-Calls parMultMatrix(nbW, sem, A, B, result) which forks nbW workers, each child computes it's rows
- After spawning the parent does nbW sem_wait calls to block until every worker is done
-Prints final matrix using printMatrix then cleans up

matrix.c
multMatrix is a triple loop that zeroes the output matrix then computes AxB = R 
parMultMatrix forks child process and has each child multiple its block of rows and the parent returns (sync happens in main.c)

1. “Why This Works" explanation: 

We slice the first matrix's rows into bands so each worker can indepdently compute it's product without busy-waiting needed. This is the power of matrix multiplication. We then place the shared results in POSIX shared memory so every child's writes go to the same arena. At the end a simple semaphore acts like a countdown latch - each child does exactly one sem_post and each parent does exactly as many sem_wait's as are workers. 
