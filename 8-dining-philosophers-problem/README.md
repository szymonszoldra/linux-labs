## PROJECT 8

The task of synchronizing the activities of the five philosophers who sit at the round table and think. Every now and then
time they get hungry and need to eat. In front of each philosopher there is a plate, and between successive plates lies
one fork. There is a fish platter in the center of the table. Fish should be eaten with two forks, so is a philosopher
he must have two forks to eat. The philosopher picking up the forks in front of him
prevents the neighbors from eating. It is assumed that the philosopher in the finite time will eat himself and put down his fork.
The solution of the problem must ensure that the following conditions are met:

- Each of the philosophers will eventually be able to eat,
- No philosophers can be distinguished (the algorithms of all five philosophers concerning forks lifting and depositing must be the same).

Typically, 3 solutions to the problem of the five philosophers are considered:

- Deadlockable solution; a hungry philosopher waits until one of his forks (e.g. the left one)
  is free and picks it up, then waits until the other fork is free and picks it up as well.
  After eating, put the forks on the table. There may be times when each of the philosophers
  he will pick up one of his forks and wait for his neighbor to finish eating and put the fork down.
  As each of the philosophers is waiting for the same event, a blockage will appear.
- Solution with the possibility of starvation; the hungry philosopher picks up two forks simultaneously
  whenever they are free. If one of the philosophers will have voracious neighbors who
  they will mainly be engaged in food, so that at any moment at least one of them will be
  he ate, such a situation will lead to the starvation of this philosopher.
- Optimal solution; philosophers alone cannot solve the problem of eating if
  neither of them is to be awarded. An external arbitrator (butler) is needed to decide,
  which philosophers will take precedence in disputed cases. The valet ensures that
  at any given moment no more than four philosophers competed for forks, lifting them sequentially
  (as in solution 1). Then at least one of the philosophers will always be able to raise
  fork and eat. If five philosophers want to eat at once, the valet will stop one of them
  until one of the other four is done eating.

Implement all three solutions to the problem of the five philosophers using semaphores.
Demonstrate the phenomena of blockage and starvation, as well as their absence in a correct solution.
