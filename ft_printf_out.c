/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_out.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oruban <oruban@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/26 18:06:11 by oruban            #+#    #+#             */
/*   Updated: 2024/04/26 20:23:20 by oruban           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

// 	pthread_mutex_lock(&(philo->args->print_mtx));
// 	pthread_mutex_unlock(&philo->args->print_mtx);
void	*ft_printf_out(t_philo *philo, char *str)
{
	pthread_mutex_lock(&(philo->args->print_mtx));
	if (issomeone_dead(philo->args) || !is_alive(philo, 1))
		return (pthread_mutex_unlock(&philo->args->print_mtx), NULL);
	printf("%ld %d %s\n", get_time(philo->args->time), philo->id + 1, str);
	pthread_mutex_unlock(&philo->args->print_mtx);
	return (philo);
}