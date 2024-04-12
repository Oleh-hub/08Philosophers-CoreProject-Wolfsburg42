/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oruban <oruban@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/29 15:14:18 by oruban            #+#    #+#             */
/*   Updated: 2024/04/12 15:34:11 by oruban           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void ft_printf_out(long int time, t_philo *philo, char *str)
{
	pthread_mutex_lock(&(philo->args->print_mid));
	printf("%ld %d %s\n", time, philo->id, str);
	pthread_mutex_unlock(&philo->args->print_mid);
}

// returns time, passed sincse 1.01.1970 in ms
static long int time_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

// sleeps for the time, indicated by variable time in ms
// usleep > 10 leads to delays of ft_msleep in 1 milliseconds for 
void ft_msleep(long int time) 
{
	long int	start;

	start = time_ms();
	while (1)
    {
        usleep(10);
        if (time_ms() - start >= time)
            break;
    }
}

// philosophers life cycle
// function, that is executed by the thread of the philosopher
void	*phl_thrd(void	*arg)
{
	t_philo	*philo;
	int		i;
	// long int tmp;
	
	philo = (t_philo *)arg;
	philo->tm_start = time_ms();
	i = 0;
	while(1)
	{
		ft_printf_out(time_ms() - philo->tm_start, philo, "is eating");
		ft_msleep(philo->args->t2eat_p);
		ft_printf_out(time_ms() - philo->tm_start, philo, "is sleeping");
		ft_msleep(philo->args->t2slp_p);
		ft_printf_out(time_ms() - philo->tm_start, philo, "is thinking");
		ft_msleep(200);
		i++;
		if (philo->args->times_p && i == philo->args->times_p)
			break ;
	}
	return (NULL);
}

int	main(void)
{
	t_args		args;
	int			i;
	t_philo		*philo;

	// struct args initialization
	args.numbr_p = 5;			// number_of_philosophers
	args.t2eat_p = 200; 		// time_to_eat_in_ms
	args.t2slp_p = 200; 		// time_to_sleep_in_ms
	args.times_p = 5;
	if (pthread_mutex_init(&args.print_mid, NULL))
		return (1);
	philo = calloc(args.numbr_p, sizeof(t_philo));
	if (!philo)
		return (pthread_mutex_destroy(&args.print_mid), 3);
	philo->args = &args;
	i = 0;
	while (i < args.numbr_p)
	{
		philo[i].id = i;
		philo[i].args = &args;
		if (pthread_create(&philo[i].thread_id, NULL, phl_thrd, (void *) &philo[i]))
			return (pthread_mutex_destroy(&args.print_mid), free(philo), 3);
		i++;
	}
	i = 0;
	while (i < args.numbr_p)
	{
		pthread_join(philo[i].thread_id, NULL);
		printf("Thread of philosopher %d is joined\n", i);
		i++;
	}
	pthread_mutex_destroy(&args.print_mid);
	free(philo);
	return (0);
}
