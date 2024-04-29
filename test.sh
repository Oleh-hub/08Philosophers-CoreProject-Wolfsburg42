# Subject"Do not test with time_to_die or time_to_eat or time_to_sleep set to
#  values lower than 60 ms."


#!/bin/bash

# ./philo 1 800 200 200

# leaks --atExit -- ./philo 4 310 200 100
for i in {1..10}
do
   echo " "
    ./philo 4 199 200 100 | grep died
    ./philo 4 290 200 100 | grep died 
    ./philo 4 301 200 100 | grep died
    ./philo 4 399 200 100 | grep died
    ./philo 5 599 200 100 | grep died  
    ./philo 5 590 200 100 | grep died 
    ./philo 5 490 200 100 | grep died 
done
    # ./philo 5 490 200 100
# everlasting loops:
# the above given subject demand sets the DELTA of project PARAMETERS sensibility to 60 ms
# therefoe the following tests are too strict and not relevant:
    # ./philo 200 1010 500 500
    # ./philo 200 410 200 200
    # ./philo 200 210 100 100 # 2k 

# eval tests
# ./philo 5 800 200 200 7
# ./philo 5 800 200 200
# ./philo 4 410 200 200
# ./philo 4 310 200 100
