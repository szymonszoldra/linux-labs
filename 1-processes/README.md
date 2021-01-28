## PROJECT 1

Write a program that lists PID, PPID, UID, GID identifiers for a given process (first.c).
Call the fork() function 3 times eg in a loop and print the above identifiers for all processes
posterity. Based on the results, draw a "family tree" of the processes being created. How much is created
processes and why (second.c).
Modify the previous program so that messages from child processes are printed by
program run by exec() function. In the parent process, implement the wait() function (process
parent waits for all child processes to terminate, prints a message which process
child has ended and with what status). How many processes are being created now and why? For all functions
system (fork(), execl(), wait()) implement error handling using the perror() library function
(third.c).
