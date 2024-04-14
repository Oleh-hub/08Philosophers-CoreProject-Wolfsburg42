/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oruban <oruban@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/29 15:14:18 by oruban            #+#    #+#             */
/*   Updated: 2024/04/14 19:05:20 by oruban           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

// returns time, passed sincse 1.01.1970 in ms
static long int	time_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

// sleeps for the time, indicated by variable time in ms
// usleep > 10 leads 2 ft_msleep  delays, 1 ms for every 200 ms twice in 5 cases
void	ft_msleep(long int time)
{
	long int	start;

	start = time_ms();
	while (1)
	{
		usleep(10);
		if (time_ms() - start >= time)
			break ;
	}
}

long	get_time(struct timeval time)
{
	struct timeval	now;
	int				diff;

	gettimeofday(&now, NULL);
	diff = (now.tv_sec * 1000 + now.tv_usec / 1000) - (time.tv_sec * 1000
			+ time.tv_usec / 1000);
	return (diff);
}

// mutexed print out the philosopher's actions
static void	ft_printf_out(t_philo *philo, char *str)
{
	pthread_mutex_lock(&(philo->args->print_mid));
	printf("%ld %d %s\n", get_time(philo->args->time), philo->id, str);
	pthread_mutex_unlock(&philo->args->print_mid);
}

// philosophers life cycle
// function, that is executed by the thread of the philosopher
// the last philosopher takes the first fork and then the fork of philosopher [0]
void	*phl_thrd(void	*arg)
{
	t_philo	*philo;
	int		i;

	philo = (t_philo *)arg;
	philo->tm_start = time_ms(); //this time the mixed up output, I think
	philo->tm_lmeal = time_ms();
	i = 0;
	while (1)
	{
		pthread_mutex_lock(&philo->args->fork_m[philo->id]);
		if (philo->id == philo->args->numbr_p - 1)
			pthread_mutex_lock(&philo->args->fork_m[0]);
		else
			pthread_mutex_lock(&philo->args->fork_m[philo->id + 1]);
		if (!(philo->args->fork[philo->id] || philo->args->fork[(philo->id + 1)]))
		{
			philo->args->fork[philo->id] = 1;
			ft_printf_out(philo, "has taken the left fork");
			philo->args->fork[philo->id + 1] = 1;
			ft_printf_out( philo, "has taken the right fork");
			ft_printf_out(philo, "is eating");
			ft_msleep(philo->args->t2eat_p);
			philo->args->fork[philo->id] = 0;
			philo->args->fork[philo->id + 1] = 0;
			pthread_mutex_unlock(&philo->args->fork_m[philo->id]);
			if (philo->id == philo->args->numbr_p - 1)
				pthread_mutex_unlock(&philo->args->fork_m[0]);
			else
				pthread_mutex_unlock(&philo->args->fork_m[philo->id + 1]);
			ft_printf_out(philo, "is sleeping");
			ft_msleep(philo->args->t2slp_p);
		}
		else
		{
			pthread_mutex_unlock(&philo->args->fork_m[philo->id]);
			if (philo->id == philo->args->numbr_p - 1)
				pthread_mutex_unlock(&philo->args->fork_m[0]);
			else
				pthread_mutex_unlock(&philo->args->fork_m[philo->id + 1]);
			ft_printf_out(philo, "is thinking");
		}
		i++;
		if (philo->args->times_p && i == philo->args->times_p)
			break ;
	}
	return (NULL);
}

t_args	*args_init(t_args *args)
{
	int			i;

	args->numbr_p = 5;
	args->t2eat_p = 200;
	args->t2slp_p = 200;
	args->times_p = 5;
	args->fork = calloc(args->numbr_p, sizeof(int));
	if (!args->fork)
		return (NULL);
	args->fork_m = calloc(args->numbr_p, sizeof(pthread_mutex_t));
	if (!args->fork_m)
		return (free(args->fork), NULL);
	if (pthread_mutex_init(&args->print_mid, NULL))
		return (free(args->fork), free(args->fork_m), NULL);
	i = -1;
	while (++i < args->numbr_p)
	{
		if (pthread_mutex_init(&args->fork_m[i], NULL))
		{
			while (--i >= 0)
				pthread_mutex_destroy(&args->fork_m[i]);
			pthread_mutex_destroy(&args->print_mid);
			return (free(args->fork), free(args->fork_m), NULL);
		}
	}
	gettimeofday(&args->time, NULL);
	return (args);
}

void	args_destroy(t_args *args)
{
	int	i;

	i = -1;
	while (++i < args->numbr_p)
		pthread_mutex_destroy(&args->fork_m[i]);
	pthread_mutex_destroy(&args->print_mid);
	free(args->fork);
	free(args->fork_m);
}

int	main(void)
{
	t_args		args;
	int			i;
	t_philo		*philo;

	if (!args_init(&args))
		return (1);
	philo = calloc(args.numbr_p, sizeof(t_philo));
	if (!philo)
		return (args_destroy(&args), 3);
	philo->args = &args;
	i = -1;
	while (++i < args.numbr_p)
	{
		philo[i].id = i;
		philo[i].args = &args;
		if (pthread_create(&philo[i].thread_id, NULL, phl_thrd,
				(void *) &philo[i]))
			return (args_destroy(&args), free(philo), 4);
	}
	i = -1;
	while (++i < args.numbr_p)
	{
		pthread_join(philo[i].thread_id, NULL);
		printf("Thread of philosopher %d is joined\n", i);
	}
	args_destroy (&args);
	free (philo);
	return (0);
}
