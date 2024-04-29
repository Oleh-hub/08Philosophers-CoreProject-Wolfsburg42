/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oruban <oruban@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/29 15:14:18 by oruban            #+#    #+#             */
/*   Updated: 2024/04/29 18:43:28 by oruban           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// https://nafuka11.github.io/philosophers-visualizer/

#include "philo.h"

// check if the philosopher is alive
int	is_alive(t_philo *philo, int isprintmutexed)
{
	if (get_time(philo->tm_lmeal) >= philo->args->t2die_p)
	{
		pthread_mutex_lock(&(philo->args->died_status));
		philo->args->died = 1;
		pthread_mutex_unlock(&philo->args->died_status);
		if (isprintmutexed)
			printf("%ld %d %s\n", get_time(philo->args->time), philo->id + 1,
				"died");
		else
		{
			pthread_mutex_lock(&(philo->args->print_mtx));
			printf("%ld %d %s\n", get_time(philo->args->time), philo->id + 1,
				"died");
			pthread_mutex_unlock(&philo->args->print_mtx);
		}
		return (0);
	}
	return (1);
}

//check if someone is alerady dead
int	issomeone_dead(t_args *args)
{
	pthread_mutex_lock(&(args->died_status));
	if (args->died)
	{
		pthread_mutex_unlock(&args->died_status);
		return (1);
	}
	pthread_mutex_unlock(&args->died_status);
	return (0);
}

// the case when there is only one philosopher (args.numbr_p == 1)
void	onephilo_case(t_args *args)
{
	printf("0 1 has taken a fork\n");
	ft_msleep(args->t2die_p);
	printf("%i 1 has died\n", args->t2die_p);
	args_destroy(args);
}

// struct s_philo initiation + threads creation.
// the function creates the threads for the philosophers
int	philo_ini(t_philo *philo, t_args *args)
{
	int	i;

	philo->args = args;
	i = -1;
	while (++i < args->numbr_p)
	{
		philo[i].id = i;
		philo[i].args = args;
		philo[i].tm_lmeal = args->time;
		if (pthread_create(&philo[i].thread_id, NULL, (t_threadfun) phl_thrd,
				(void *) &philo[i]))
			return (1);
	}
	return (0);
}

int	main(int ac, char **av)
{
	t_args		args;
	int			i;
	t_philo		*philo;

	if (!(ac == 5 || ac == 6) || !av_check(av))
		return (printf("parameters: 4 or 5 positive numbers\n"), 2);
	if (!args_init(&args, av))
		return (1);
	if (args.numbr_p == 1)
		return (onephilo_case(&args), 0);
	philo = ft_calloc(args.numbr_p, sizeof(t_philo));
	if (!philo)
		return (args_destroy(&args), 3);
	if (philo_ini(philo, &args))
		return (args_destroy(&args), free(philo), 5);
	i = -1;
	while (++i < args.numbr_p)
		pthread_join(philo[i].thread_id, NULL);
	args_destroy (&args);
	free (philo);
	return (0);
}
