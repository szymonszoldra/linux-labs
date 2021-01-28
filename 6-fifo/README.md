## PROJECT 6

Using named pipes (fifos) to generalize the producer-consumer task to multiple producers and
many consumers. Use one named link in the task.
The project consists of two programs (files): producer and consumer. Run programs
independently.
The argument for calling the producer program is the number of producers and the number of characters
"Produced" by each manufacturer (e.g ./producer 50 1000). Producer program
creates a specified number of processes that write data to a pipeline and then waits for them
end.
The argument for calling the consumer program is the number of consumers (e.g. ./consumer 30).
The consumer program creates a specified number of processes that read data from the pipeline and then
waiting for their completion.
The programs check the permissible limit of processes that the user can run in a given
when and in the case when the limit is greater than or equal to the number of created processes, it starts
request.
Each producer draws the number of characters specified by the second argument. Drawn
writes characters - one character each - to the pipe and its data file (e.g. we_1250.txt, where
the given number is the pid of the process that created the file). Each consumer reads one
character from the pipe and saves it in its results file (e.g. wy_1320.txt). When finished, the requested number
characters in all files we\_\*.txt must equal the number of characters in all files in wy\_\*.txt.
After the task is completed (the last character is downloaded from the link), one of the processes deletes the created link.
