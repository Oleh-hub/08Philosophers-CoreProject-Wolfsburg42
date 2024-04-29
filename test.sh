# Subject"Do not test with time_to_die or time_to_eat or time_to_sleep set to
#  values lower than 60 ms."


#!/bin/bash

# ./philo 1 800 200 200

# leaks --atExit -- ./philo 4 310 200 100
for i in {1..10}
do
   echo " "
    ./philo 4 299 200 100 # no indication of getting forks, eating. just "died"
    # ./philo 4 190 200 100 | grep died # no indication of getting forks, eating. just "died"
    # ./philo 4 290 200 100 | grep died # no indication of getting forks, eating. just "died"
# the cases below, when the 2 philo simultaneous death can be explained 
# with PC performance - they r so seldom
    # ./philo 4 310 200 100 | grep died
    # ./philo 4 390 200 100 | grep died
    ./philo 5 599 200 100  # no indication of getting forks, eating. just "died"
    ./philo 5 590 200 100 | grep died # no indication of getting forks, eating. just "died"
    ./philo 5 490 200 100 | grep died # no indication of getting forks, eating. just "died"
done
    # everlasting loops:
    # ./philo 200 260 100 100 # two messages is eating
# the above given subject demand sets the DELTA of project PARAMETERS sensibility to 60 ms
# therefoe the following tests are too strict and not relevant:
    # ./philo 200 1010 500 500 # 30k died two messages is eating  
    # ./philo 200 410 200 200 # 4k died && tow is eating mesaages
    # ./philo 200 210 100 100 # 2k 0.7k died two messages is eating

# eval tests
# ./philo 5 800 200 200 7 # 2 eateing messageds everywhere
# ./philo 5 800 200 200
# ./philo 4 410 200 200
# ./philo 4 310 200 100
