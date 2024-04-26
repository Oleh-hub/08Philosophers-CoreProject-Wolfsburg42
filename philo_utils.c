/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oruban <oruban@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/29 16:40:04 by oruban            #+#    #+#             */
/*   Updated: 2024/04/26 20:23:41 by oruban           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

// Makes delay b4 comming to fork mutexes if the philo should die:
// 1 - during his eating/sleeping for every case
// (time_to_die < time_to_eat + time_to_sleep or time_to_die < 2 * time_to_eat)
// 2 - during other philos eating in case the number_of_philosophers is uneven
// (numbr_p % 2 && time_to_die < 3 * time_to_eat)
	// DEBIGGING:
	// if (philo->id == 3 || philo->id == 1) // tracing start
	// {
	// pthread_mutex_lock(&(philo->args->print_mtx));
	// printf("%l d%d%s\n", get_time(philo->args->time), philo->id + 1, "TEST");
	// pthread_mutex_unlock(&philo->args->print_mtx);
	// }									// tracing end
static void	wait4death(t_philo *philo, int i)
{
	long	last_breath;

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
}

// philo takes forks, eats and and than waits - created bcause of norminette
// RETURNS:
// (void *)philo or NULL if the gettimeofday fails
static void	*eating(t_philo *philo)
{
	philo->args->fork[philo->id] = 1;
	if (!ft_printf_out(philo, "has taken a fork"))
		return (NULL);
	philo->args->fork[(philo->id + 1) % philo->args->numbr_p] = 1;
	if (!ft_printf_out(philo, "has taken a fork"))
		return (NULL);
	if (gettimeofday(&philo->tm_lmeal, NULL) == -1)
		return (NULL);
	ft_printf_out(philo, "is eating");
	if (!ft_printf_out(philo, "is eating"))
		return (NULL);
	if (philo->args->t2eat_p < philo->args->t2die_p)
		ft_msleep(philo->args->t2eat_p);
	else
		ft_msleep(philo->args->t2die_p);
	return ((void *)philo);
}

// philo's life with taken forks :) - created becasue of norminette
// RETURNS:
// if the philo dies during eating/sleeping NULL is returned
// otherwise tphilo
static void	*survived_eat_sleep(t_philo *philo)
{
	if (!eating(philo))
		return (NULL);
	// if (issomeone_dead(philo->args) || !is_alive(philo))
	// 		return (philoforks_mutexs_unlock(philo), NULL);
	philo->args->fork[philo->id] = 0;
	philo->args->fork[(philo->id + 1) % philo->args->numbr_p] = 0;
	philoforks_mutexs_unlock(philo);
	if (!ft_printf_out(philo, "is sleeping"))
		return (NULL);
	if (philo->args->t2slp_p < (philo->args->t2die_p - philo->args->t2eat_p))
		ft_msleep(philo->args->t2slp_p);
	else
		ft_msleep((philo->args->t2die_p - philo->args->t2eat_p));
	// if (issomeone_dead(philo->args)	|| !is_alive(philo))
	// 	return (NULL);
	return ((void *)philo);
}

// jsut lokcs mutexes for both forkes of the philo given as a parameter
static void	philoforks_mutexs_lock(t_philo *philo)
{
	pthread_mutex_lock(&philo->args->fork_m[philo->id]);
	pthread_mutex_lock(&philo->args->fork_m[(philo->id + 1)
		% philo->args->numbr_p]);
}

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
void	*phl_thrd(t_philo *philo)
{
	int		i;

	i = -1;
	while (1)
	{
		wait4death(philo, i);
		if (issomeone_dead(philo->args) || !is_alive(philo, 0))
			return (NULL);
		if (++i == philo->args->times_p && philo->args->times_p)
			break ;
		if (philo->id % 2 && i == 0)
			ft_msleep(10);
		philoforks_mutexs_lock(philo);
		if (issomeone_dead(philo->args) || !is_alive(philo, 0))
			return (philoforks_mutexs_unlock(philo), NULL);
		if (!(philo->args->fork[philo->id] || philo->args->fork[(philo->id + 1)
					% philo->args->numbr_p]) && !survived_eat_sleep(philo))
			return (NULL);
		else
			philoforks_mutexs_unlock(philo);
		if (!ft_printf_out(philo, "is thinking"))
			return (NULL);
	}
	return (NULL);
}
