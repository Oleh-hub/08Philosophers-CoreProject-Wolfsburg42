## Philosophers
## I never thought philosophy would be so deadly

## WHAT IS THE GOAL OF THE PROJECT?

The goal of the project is to implement a *dining philosophers problem* by using
threads.

Here are the things needed to follow to succeed in this assignment:

• One or more philosophers sit at a round table.
There is a large bowl of spaghetti in the middle of the table.

• The philosophers alternatively eat, think, or sleep.
While they are eating, they are not thinking nor sleeping;
while thinking, they are not eating nor sleeping;
and, of course, while sleeping, they are not eating nor thinking.

• There are also forks on the table. There are as many forks as philosophers.

• Because serving and eating spaghetti with only one fork is very inconvenient, a
philosopher takes their right and their left forks to eat, one in each hand.

• When a philosopher has finished eating, they put their forks back on the table and
start sleeping. Once awake, they start thinking again. The simulation stops when
a philosopher dies of starvation.

• Every philosopher needs to eat and should never starve.

• Philosophers don’t speak with each other.

• Philosophers don’t know if another philosopher is about to die.

• No need to say that philosophers should avoid dying!

# Philosophers Resources
https://philosophers.simple.ink/
## Philosophers 42 Guide— “The Dining Philosophers Problem”: 
https://medium.com/@ruinadd/philosophers-42-guide-the-dining-philosophers-problem-893a24bc0fe2 

# To Run Philosophers

1 - Copy and paste this in iTerm/Terminal `git clone https://github.com/pasqualerossi/Philosophers.git && cd Philosophers && make` <br>

2 - `./philo [number_of_philosophers] [time_to_die] [time_to_eat] [time_to_sleep] [number_of_times_each_philosopher_must_eat]`

Examples:

`./philo 1 800 200 200` - Philosopher should not eat and should die. <br>
`./philo 5 800 200 200` - No Philosopher should die. <br>
`./philo 5 800 200 200 7` - No Philosopher should die and the simulation should stop when every philosopher has eaten at least 7 times. <br>
`./philo 4 410 200 200` - No Philosopher should die. <br>
`./philo 4 310 200 100` - One Philosopher should die. 
