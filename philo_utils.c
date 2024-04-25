/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oruban <oruban@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/29 16:40:04 by oruban            #+#    #+#             */
/*   Updated: 2024/04/25 09:31:44 by oruban           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

// philosophers life cycle theread simulation
// function, that is executed by the thread of the philosopher
// the last philosopher takes the first fork and then the fork of philosopher[0]
// VARIABLES:
// long	last_breath; - case when philo lives b4 the milstone event (eat, sleep)
// in the while loop there are the checks if issomeone_ded and if the philo
// is_alive (philo)
// in the cases when the philo should die before teh next event (eat, sleep) the
// last_breath delay is calculated in milliseconds and executed with ft_msleep 
// cases r pretty clear if you remember that
// numbr_p is number_of_philosophers - ./philo 1stParameter
// t2die_p is time_2_die - ./philo_2ndParameter
// t2eat_p is time_2_eat - ./philo_3rdParameter
// t2slp_p is time_2_sleep - ./philo_4thParameter
// tm_lmeal is the time_of_last_meal start
// "if (i >=  0 && numbr_p % 2 && t2die_p < 3 * t2eat_p)" is for cases with 
// uneven number of philos like "./philo 3 1000 350 100" or 
// "./philo 5 490 200 100"
// it is worth 2 b noted that pthread_mutex_lock funciton is the places in the
//  code where the the porgram (the philosopher) waits till the mutex(e.g. fork)
// is freed and does not check if he is already dead. 
// E.g.	pthread_mutex_lock(&philo->args->fork_m[philo->id]);

// if the philo dies during the eating or sleeping time

void	*phl_thrd(t_philo *philo)
{
	int		i;
	long	last_breath;

	i = -1;
	while (1)
	{
		if (philo->args->t2die_p < philo->args->t2eat_p + philo->args->t2slp_p
			|| philo->args->t2die_p < 2 * philo->args->t2eat_p)
		{
			last_breath = philo->args->t2die_p - get_time(philo->tm_lmeal);
			if ((i >= 0 && last_breath < philo->args->t2eat_p)
				|| (philo->args->numbr_p % 2 && philo->id
					== philo->args->numbr_p - 2))
				ft_msleep(last_breath);
		}
		if (i >= 0 && philo->args->numbr_p % 2
			&& philo->args->t2die_p < 3 * philo->args->t2eat_p)
		{
			last_breath = philo->args->t2die_p - get_time(philo->tm_lmeal);
			ft_msleep(last_breath);
		}
		if (issomeone_dead(philo->args))
			return (NULL);
		if (!is_alive(philo))
			return (NULL);
		if (++i == philo->args->times_p && philo->args->times_p)
			break ;
		if (philo->id % 2 && i == 0)
		{
			ft_printf_out(philo, "is thinking");
			ft_msleep(3);
		}
		pthread_mutex_lock(&philo->args->fork_m[philo->id]);
		pthread_mutex_lock(&philo->args->fork_m[(philo->id + 1)
			% philo->args->numbr_p]);
		if (issomeone_dead(philo->args))
			return (forks_mutex_unlock(philo), NULL);
		if (!is_alive(philo))
			return (forks_mutex_unlock(philo), NULL);
		if (!(philo->args->fork[philo->id] || philo->args->fork[(philo->id + 1)
					% philo->args->numbr_p]))
		{
			philo->args->fork[philo->id] = 1;
			ft_printf_out(philo, "has taken a fork");
			philo->args->fork[(philo->id + 1) % philo->args->numbr_p] = 1;
			ft_printf_out(philo, "has taken a fork");
			if (gettimeofday(&philo->tm_lmeal, NULL) == -1)
				return (NULL);
			ft_printf_out(philo, "is eating");
			// check if the philo has to die during eating
			if (philo->args->t2eat_p < philo->args->t2die_p)
				ft_msleep(philo->args->t2eat_p);
			else
				ft_msleep(philo->args->t2die_p);
			if (!is_alive(philo))
				return (forks_mutex_unlock(philo), NULL);
			philo->args->fork[philo->id] = 0;
			philo->args->fork[(philo->id + 1) % philo->args->numbr_p] = 0;
			forks_mutex_unlock(philo);
			if (issomeone_dead(philo->args))
				return (NULL);
			ft_printf_out(philo, "is sleeping");
			// check if the philo has to die during sleeping
			if (philo->args->t2slp_p < (philo->args->t2die_p
					- philo->args->t2eat_p))
				ft_msleep(philo->args->t2slp_p);
			else
				ft_msleep((philo->args->t2die_p - philo->args->t2eat_p));
			if (issomeone_dead(philo->args))
				return (NULL);
			if (!is_alive(philo))
				return (NULL);
		}
		else
		{
			pthread_mutex_unlock(&philo->args->fork_m[philo->id]);
			pthread_mutex_unlock(&philo->args->fork_m[(philo->id + 1)
				% philo->args->numbr_p]);
		}
		ft_printf_out(philo, "is thinking");
	}
	return (NULL);
}
