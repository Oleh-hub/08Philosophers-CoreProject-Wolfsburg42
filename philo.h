/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oruban <oruban@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/29 16:39:54 by oruban            #+#    #+#             */
/*   Updated: 2024/04/29 18:46:16 by oruban           ###   ########.fr       */
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
	int				times_p; // number_of_times_each_philosopher_must_eat
	int				died;		// flag of the death of a philosopher
	int				*fork;	// the array of forks may == 0 or 1 only. Needed 
	// because of subject statement: U "should protect the forks state with a
	// mutex for each of them."
	pthread_mutex_t	died_status; // 2 allow change 'int died' without data race
	pthread_mutex_t	*fork_m; // an array of mutexes (fork[numbr_p]) 4 every fork
	pthread_mutex_t	print_mtx; // mutex id 2 garantee no data race during printf
	struct timeval	time; // start time of the simulation (a la Beqa)
}				t_args;

typedef struct s_philo
{
	int				id;			// id of the philosopher
	// long int	tm_start;	// start time of the philosophers thread
	// long int	tm_lmeal;	// time of the last meal || hunger life span begin
	struct timeval	tm_lmeal; // time of the last meal start || life span begin
	pthread_t		thread_id;	// thread id of the philosopher
	t_args			*args;		// pointer to the arguments structure
}				t_philo;

////Thobias' idea to avoid dataype casting in the thread function:
////Thobias' idea to avoid dataype casting in the thread function:
// 'void	*phl_thrd(t_philo *philo)' instead of 'void	*phl_thrd(void *philo)'
// called from philo_ini()
typedef void	*(*t_threadfun)(void *);

// philo_utils2.c
void	*ft_calloc(size_t num, size_t size);
int		ft_itoa(char *av);
void	ft_msleep(long int time);
long	get_time(struct timeval time);
// philo_utils1.c
void	args_destroy(t_args *args);
void	philoforks_mutexs_unlock(t_philo *philo);
t_args	*args_init(t_args *args, char **av);
void	args_destroy(t_args *args);
int		av_check(char **av);
// philo_utils.c
void	*phl_thrd(t_philo *philo);
// philo.c
// void	ft_printf_out(t_philo *philo, char *str);
void	*ft_printf_out(t_philo *philo, char *str);
int		issomeone_dead(t_args *args);
// int		is_alive(t_philo *philo);
void	*tracing(t_philo *philo, char *str); // tracing function
int		is_alive(t_philo *philo, int isprintmutexed);
#endif /* PHILO_H */