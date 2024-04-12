/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oruban <oruban@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/29 16:39:54 by oruban            #+#    #+#             */
/*   Updated: 2024/04/11 16:45:00 by oruban           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H
# include <stdio.h> // printf
# include <pthread.h> // pthread_create, pthread_join, pthread_mutex_init, etc
# include <unistd.h> // sleep
# include <stdlib.h> // malloc, free
# include <string.h> // strerror
#include <sys/time.h>	// gettimeofday

typedef struct s_args
{
	int				numbr_p; // number_of_philosophers
	int				t2eat_p; // time_to_eat_in_ms
	int				t2slp_p; // time_to_sleep_in_ms
	int				times_p; //number_of_times_each_philosopher_must_eat
	pthread_mutex_t	print_mid; // mutex id to garantee no data race during printf
}				t_args;

typedef struct s_philo
{
	int			id; 		// id of the philosopher
	long int	tm_start; 	// start time of the philosophers thread
	pthread_t	thread_id; 	// thread id of the philosopher
	t_args		*args;		// pointer to the arguments structure
}				t_philo;

void	prnt(void);
#endif /* PHILO_H */