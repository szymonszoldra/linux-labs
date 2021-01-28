## PROJECT 5

By using unnamed pipes to generalize the producer-consumer task to many producers and
many consumers. Use one unnamed link in the task.
The argument for calling the program is the number of consumers, the number of producers and the number of characters
"Produced" by each manufacturer (eg ./prog 10 50 1000). The program checks
permissible limit of processes that a user can run at a given moment (e.g. with
function popen()) and in the case when the limit is greater than or equal to the number of created processes,
starts the task.
Each producer draws the number of characters specified by the third argument. Drawn
writes characters - one character each - to the pipe and its data file (e.g. we_1250.txt, where
the given number is the pid of the process that created the file). Each consumer reads one
character from the pipe and saves it in its results file (e.g. wy_1320.txt). When finished, the requested number
characters in all files we\_\*.txt must equal the number of characters in all files in wy\_\*.txt.

Program error handling based on the perror() function and the errno variable for system functions.
