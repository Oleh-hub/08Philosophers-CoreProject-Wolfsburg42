/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils1.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oruban <oruban@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/29 16:40:04 by oruban            #+#    #+#             */
/*   Updated: 2024/04/25 11:08:42 by oruban           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

//fork mutexes initialization = number_of_philos
static t_args	*allforks_mutexs_ini(t_args *args)
{
	int	i;

	i = -1;
	while (++i < args->numbr_p)
	{
		if (pthread_mutex_init(&args->fork_m[i], NULL))
		{
			while (--i >= 0)
				pthread_mutex_destroy(&args->fork_m[i]);
			pthread_mutex_destroy(&args->print_mtx);
			pthread_mutex_destroy(&args->died_status);
			return (free(args->fork), free(args->fork_m), NULL);
		}
	}
	return (args);
}

// unlocking philos forks considering the last philosopher
void	philoforks_mutexs_unlock(t_philo *philo)
{
	pthread_mutex_unlock(&philo->args->fork_m[philo->id]);
	pthread_mutex_unlock(&philo->args->fork_m[(philo->id + 1)
		% philo->args->numbr_p]);
}

// init the arguments structure
t_args	*args_init(t_args *args, char **av)
{
	args->numbr_p = ft_itoa(av[1]);
	args->t2die_p = ft_itoa(av[2]);
	args->t2eat_p = ft_itoa(av[3]);
	args->t2slp_p = ft_itoa(av[4]);
	args->times_p = ft_itoa(av[5]);
	args->died = 0;
	args->fork = ft_calloc(args->numbr_p, sizeof(int));
	if (!args->fork)
		return (NULL);
	args->fork_m = ft_calloc(args->numbr_p, sizeof(pthread_mutex_t));
	if (!args->fork_m)
		return (free(args->fork), NULL);
	if (pthread_mutex_init(&args->print_mtx, NULL))
		return (free(args->fork), free(args->fork_m), NULL);
	if (pthread_mutex_init(&args->died_status, NULL))
		return (pthread_mutex_destroy(&args->print_mtx), free(args->fork),
			free(args->fork_m), NULL);
	if (!allforks_mutexs_ini(args))
		return (NULL);
	if (gettimeofday(&args->time, NULL) == -1)
		return (args_destroy(args), NULL);
	return (args);
}

// destroy all mutexes and free the memory
void	args_destroy(t_args *args)
{
	int	i;

	i = -1;
	while (++i < args->numbr_p)
		pthread_mutex_destroy(&args->fork_m[i]);
	pthread_mutex_destroy(&args->print_mtx);
	pthread_mutex_destroy(&args->died_status);
	free(args->fork);
	free(args->fork_m);
}

// check if the arguments are numeric
int	av_check(char **av)
{
	int	i;
	int	j;

	i = 0;
	while (av[++i])
	{
		j = -1;
		while (av[i][++j])
		{
			if (!(av[i][j] >= '0' && av[i][j] <= '9'))
				return (0);
		}
	}
	return (1);
}
