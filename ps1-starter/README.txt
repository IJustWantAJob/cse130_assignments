3. Code description

Q1. fork() and exec()

1. “Why This Works" explanation: 
run2.c forks twice and creates 2 child processes.
The first uses execlp to run the command in argv[1] (literal argument) and the second uses execvp to run argv[3] with all the other arguments. Each chidl replaces its process image and the parent waits for both using waitpid then prints exit information using WIFEXITED and WEXITSTATUS.

2. Testing strategy and inputs:
I tested simple stuff like ./run2 echo Hello echo world, ./run2 cat run2.c ls /dev (file operations), and fake cases like ./run2 fakeeeeeee argumeneeetttt ls / and tests (concurrency) like ./run2 sleep 1 echo Now.

3. Output analysis:
I tested each of the commands a few times (like echo Hello echo world) to amke sure the output was unpredictable and made sure it was followed by exited=1 and exitstatus=0 (successful exits). For failure I made sure it printed an error and exitstatus=1.


Q2. Pipes

1. “Why This Works" explanation: 
pipes.c sets up 2 pipes and forks 3 children to implement ls /dev | xargs | cut. Each child redirects stdin/out using dup2 to link the pipeline stages before calling execlp. The parent closes all pipe ends and waits, together this recreates the 3 stage shell pipeline.

2. Testing strategy and inputs:
I tested ./pipes 1 5, ./pipes 1 15, ./pipes 10 20 to make sure the ranges were correct. I also made sure of trying out of bounds tests using larger values (100, 200) and invalid arguments were properlly throwing errors. 

3. Output analysis:
Depending on the argument passed in, the function prints out the specific fields from /dev (correct pipe chaining and cut). Out of bound has no output, which makes sense, and incorrect/wrong inputs print an error. There were some issues with hanging, but that's now fixed.

