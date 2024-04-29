/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_out.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oruban <oruban@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/26 18:06:11 by oruban            #+#    #+#             */
/*   Updated: 2024/04/29 15:02:12 by oruban           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*ft_printf_out(t_philo *philo, char *str)
{
	pthread_mutex_lock(&(philo->args->print_mtx));
	if (issomeone_dead(philo->args) || !is_alive(philo, 1))
		return (pthread_mutex_unlock(&philo->args->print_mtx), NULL);
	printf("%ld %d %s\n", get_time(philo->args->time), philo->id + 1, str);
	pthread_mutex_unlock(&philo->args->print_mtx);
	return (philo);
}

// tracing funcitoin
// tracing(philo, "DEBUG"); // tracing
void	*tracing (t_philo *philo, char *str)
{ //tracing
	if (philo->id == 0 || philo->id == 2) // tracing
	{
		pthread_mutex_lock(&(philo->args->print_mtx));
		printf("%ld %d %s\n", get_time(philo->args->time), philo->id + 1, str);	//
		pthread_mutex_unlock(&philo->args->print_mtx);
	}
	return (philo);
} // tracing end