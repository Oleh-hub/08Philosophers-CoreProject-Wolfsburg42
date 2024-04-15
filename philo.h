/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oruban <oruban@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/29 16:39:54 by oruban            #+#    #+#             */
/*   Updated: 2024/04/15 20:08:21 by oruban           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H
# include <stdio.h> // printf
# include <pthread.h> // pthread_create, pthread_join, pthread_mutex_init, etc
# include <unistd.h> // sleep
# include <stdlib.h> // malloc, free
# include <string.h> // strerror
# include <sys/time.h>	// gettimeofday

typedef struct s_args
{
	int				numbr_p; // number_of_philosophers
	int				t2die_p; // time_to_die_in_ms
	int				t2eat_p; // time_to_eat_in_ms
	int				t2slp_p; // time_to_sleep_in_ms
	int				times_p; //number_of_times_each_philosopher_must_eat
	int				*fork;	// the array of forks may == 0 or 1 only. Needed 
	int				died;		// flag of the death of a philosopher
	// because U "should protect the forks state with a mutex for each of them."
	pthread_mutex_t	died_status; // 2 allow change 'int died' without data race
	pthread_mutex_t	*fork_m; // an array of mutexes (fork[numbr_p]) 4 every fork
	pthread_mutex_t	print_mid; // mutex id 2 garantee no data race during printf
	struct timeval time; // start time of the simulation (a la Beqa)
}				t_args;

typedef struct s_philo
{
	int				id;			// id of the philosopher
	// long int	tm_start;	// start time of the philosophers thread
	// long int	tm_lmeal;	// time of the last meal || hunger life span begin
	struct timeval	tm_lmeal;	// time of the last meal start || life span begin
	pthread_t		thread_id;	// thread id of the philosopher
	t_args			*args;		// pointer to the arguments structure
}				t_philo;

void	prnt(void);
#endif /* PHILO_H */