2. Code description

1. “Why This Works" explanation: 

Our code separates the three roles:
1) SQL command server
2) client front end
3) control service


1) We have 2 dedicated sockets
Port 1 (sid) handles SQL clients
Port 2 (srv) handles the shutdown command ($die!)

This was no normal queries can accidentally shutdown the server

2) We use select() and IO multiplexing
We call select() on both listening sockets in server's main loop
When srv is ready we accept(), and either shut down or reply bad command
When sid is ready we accept() and fork()

3) Using fork() and execlp(sqlite3) we have client isolation
In the child process we close the listening sockets, dupe2() the client socket onto stdin, stdout, and stderr then call execlp(sqlite3, sqlite3, foobar.db, null)

Each client gets its own sqlite3 interpreter and directory. the parent process closesthe client socket and calls cleanupDeadChildren to reap exited children

4) Shutdown and error handling
Resv "$die!" on srv triggers these:
- closing both sockets
- calling cleanupDeadChildren() 
- printing"terminated" and exiting

any other control input returns bad command [<input>] and closes that connection

