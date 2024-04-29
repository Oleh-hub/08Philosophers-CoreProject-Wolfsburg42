/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_out.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oruban <oruban@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/26 18:06:11 by oruban            #+#    #+#             */
/*   Updated: 2024/04/29 13:07:19 by oruban           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*ft_printf_out(t_philo *philo, char *str)
{
	pthread_mutex_lock(&(philo->args->print_mtx));
// 	pthread_mutex_lock(&(philo->args->print_mtx));
// 	pthread_mutex_unlock(&philo->args->print_mtx);
	// if (philo->id == 4) // tracing
	// 	printf("%ld %d %s\n", get_time(philo->args->time), philo->id + 1, "i do not know that someone is dead");	//
	if (issomeone_dead(philo->args) || !is_alive(philo, 1))
	{ // tracing
		// printf("%ld %d %s\n", get_time(philo->args->time), philo->id + 1, "i know that someone is dead");	//
		return (pthread_mutex_unlock(&philo->args->print_mtx), NULL);
	}
	printf("%ld %d %s\n", get_time(philo->args->time), philo->id + 1, str);
	pthread_mutex_unlock(&philo->args->print_mtx);
	return (philo);
}

// tracing funcitoin
// tracing(philo, "DEBUG"); // tracing
void	*tracing (t_philo *philo, char *str)
{ //tracing
	pthread_mutex_lock(&(philo->args->print_mtx));
		printf("%ld %d %s\n", get_time(philo->args->time), philo->id + 1, str);	//
	pthread_mutex_unlock(&philo->args->print_mtx);
	return (philo);
} // tracing end