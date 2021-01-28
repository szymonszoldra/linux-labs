## PROJECT 7

The task of synchronizing two groups of processes: readers and writers, competing for access to
common reading room. The reader process: every now and then reads the information gathered in the reading room and can
do it together with other readers; The writer process: every now and then he writes down his information and must
then be alone in the reading room; This task may involve the so-called the phenomenon of starvation of one of the
process groups. It occurs when the process does not resume despite having an event on
which waits any number of times. Whenever this process could be resumed,
some other pending process is selected.

Consider 2 solutions to the problem of readers and writers:

- Solution with the possibility of starvation of writers - reader priority;
- Solution with the possibility of starving the readers - the writer's priority;

Solve the problem in 2 versions using semaphores. Assume that the number of places in the reading room is
limited and set to a certain value M. The number of readers and writers can be both greater
and less than M. Write a program to generate processes for readers and writers based on
fork() and exec() functions. Demonstrate the starvation of the writer's and the reader's processes.
To simulate different program speeds, use e.g. the sleep() function with a random number of seconds.
Implement support for interrupting the semop() system call. For system functions
program error handling based on the perror() function and errno.
The main program checks the permissible limit of processes that the user may run in a given
when and in the case when the limit is greater than or equal to the number of created processes, it starts
request.
Example of calling "duplicator".

./main writers_number readers_number M_number

for example
./main 2 5 3
