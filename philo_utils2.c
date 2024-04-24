/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oruban <oruban@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/29 16:40:04 by oruban            #+#    #+#             */
/*   Updated: 2024/04/24 20:18:06 by oruban           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

// does the same as calloc 
void	*ft_calloc(size_t num, size_t size)
{
	unsigned char	*ptr;
	size_t			nsize;
	size_t			i;

	nsize = num * size;
	ptr = malloc(nsize);
	if (ptr)
	{
		i = -1;
		while (++i < nsize)
			ptr[i] = 0;
	}
	return (ptr);
}

// converts the string to an integer
int	ft_itoa(char *av)
{
	int	i;
	int	num;

	if (!av)
		return (0);
	i = -1;
	num = 0;
	while (av[++i])
		num = num * 10 + av[i] - '0';
	return (num);
}

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
		usleep(150);
		if (time_ms() - start >= time)
			break ;
	}
}

// returns ms passed since the time given as the argument
long	get_time(struct timeval time)
{
	struct timeval	now;
	int				diff;

	gettimeofday(&now, NULL);
	diff = (now.tv_sec * 1000 + now.tv_usec / 1000) - (time.tv_sec * 1000
			+ time.tv_usec / 1000);
	return (diff);
}
