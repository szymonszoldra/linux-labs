## PROJECT 3

The project consists of two programs that run independently: producer and consumer.
The producer produces the commodity (e.g. numbers, characters) and places them in a common buffer
(shared memory) that can only hold one item at a time. Consumer downloads
the commodity (without destroying the buffer) and consumes it.
For harmony to reign, two conditions must be met:

- each unit of goods produced must be consumed,
- no unit of a good can be consumed twice (even if the consumer is faster than the manufacturer).

Implement the above task using the semaphore mechanism. To demonstrate that
there has been no loss or multiplication of the goods, let the producer collect the "raw material" (numbers, characters) for
producing goods from a text file, and the consumer places the downloaded goods in another file
text. After the programs are finished (the "raw" resources are exhausted), both files
text files should be identical. To simulate different speeds of the programs, use e.g.
sleep() function with a random number of seconds. Implement a function call interrupt handler
system semop().
