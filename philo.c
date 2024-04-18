/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oruban <oruban@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/29 15:14:18 by oruban            #+#    #+#             */
/*   Updated: 2024/04/18 19:50:10 by oruban           ###   ########.fr       */
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

// mutexed print out the philosopher's actions
static void	ft_printf_out(t_philo *philo, char *str)
{
	pthread_mutex_lock(&(philo->args->print_mtx));
	printf("%ld %d %s\n", get_time(philo->args->time), philo->id + 1, str);
	pthread_mutex_unlock(&philo->args->print_mtx);
}

// check if the philosopher is alive
int is_alive(t_philo *philo)
{
	if (get_time(philo->tm_lmeal) >= philo->args->t2die_p)
	{
		pthread_mutex_lock(&(philo->args->died_status));
		philo->args->died = 1;
		pthread_mutex_unlock(&philo->args->died_status);
		ft_printf_out(philo, "has died");
		return (0);
	}
	return (1);
}

// unlocking philos forks considering the last philosopher
void forks_mutex_unlock(t_philo *philo)
{
	pthread_mutex_unlock(&philo->args->fork_m[philo->id]);
	if (philo->id == philo->args->numbr_p - 1)
		pthread_mutex_unlock(&philo->args->fork_m[0]);
	else
		pthread_mutex_unlock(&philo->args->fork_m[philo->id + 1]);
}

//check if someone is dead

int issomeone_dead(t_args *args)
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

// philosophers life cycle
// function, that is executed by the thread of the philosopher
// the last philosopher takes the first fork and then the fork of philosopher[0]
//
// long	last_breath; - case when philo lives b4 the milstone event (eat, sleep)

void	*phl_thrd(void	*arg)
{
	t_philo	*philo;
	int		i;
	long	last_breath; // case when philo lives b4 the milstone event (eat, sleep)
	
	philo = (t_philo *)arg;
	i = -1;
	while (1)
	{
		
		// here should be a condition to make a philo to die if lifspan < time2eat
		// AND he should dont die if eat + sleep < lifespan
			
		if (philo->args->t2die_p < philo->args->t2eat_p + philo->args->t2slp_p ||
			philo->args->t2die_p < 2* philo->args->t2eat_p)
		{
			// calculates what is left_to_live if life_span < tie_to_eat
			last_breath = philo->args->t2die_p - get_time(philo->tm_lmeal);
			if ((i >=  0 && last_breath < philo->args->t2eat_p) || 
				(philo->args->numbr_p % 2 && philo->id == philo->args->numbr_p - 2)) //uneven philos the one b4 last dies b4 getting a fork on a first try
				ft_msleep(last_breath);
		}
		if (issomeone_dead(philo->args)) // check if someone is dead
			return (NULL);
		if (!is_alive(philo))
			return (NULL);
		if (++i == philo->args->times_p && philo->args->times_p)
			break ;
		if (philo->id % 2 && i == 0) // uneven philos 1, 3... start  1 time with a delay
			ft_msleep(3);
		// next line this is where teh philosopher waits till the fork is frre
		// and does not check if he is already dead
		pthread_mutex_lock(&philo->args->fork_m[philo->id]);
		if (philo->id == philo->args->numbr_p - 1)
			pthread_mutex_lock(&philo->args->fork_m[0]);
		else
			pthread_mutex_lock(&philo->args->fork_m[philo->id + 1]);
		if (!is_alive(philo))
			return (forks_mutex_unlock(philo), NULL);
		if (!(philo->args->fork[philo->id] || philo->args->fork[(philo->id + 1)]))
		{
			philo->args->fork[philo->id] = 1;
			ft_printf_out(philo, "has taken the left fork");
			philo->args->fork[philo->id + 1] = 1;
			ft_printf_out( philo, "has taken the right fork");
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
			philo->args->fork[philo->id + 1] = 0;
			forks_mutex_unlock(philo);
			if (issomeone_dead(philo->args)) // check if someone is dead
				return (NULL);
			ft_printf_out(philo, "is sleeping");
			// check if the philo has to die during sleeping
			if (philo->args->t2slp_p < (philo->args->t2die_p - philo->args->t2eat_p))
				ft_msleep(philo->args->t2slp_p);
			else
				ft_msleep((philo->args->t2die_p - philo->args->t2eat_p));
			if (issomeone_dead(philo->args)) // check if someone is dead
				return (NULL);
			if (!is_alive(philo)) // is this philo still alive?
				return (NULL);
		}
		else
		{
			pthread_mutex_unlock(&philo->args->fork_m[philo->id]);
			if (philo->id == philo->args->numbr_p - 1)
				pthread_mutex_unlock(&philo->args->fork_m[0]);
			else
				pthread_mutex_unlock(&philo->args->fork_m[philo->id + 1]);
		}
		ft_printf_out(philo, "is thinking");
	}
	return (NULL);
}

// converts the string to an integer
int ft_itoa(char *av)
{
	int i;
	int num;

	if (!av)
		return (0);
	i = -1;
	num = 0;
	while (av[++i])
		num = num * 10 + av[i] - '0';
	return (num);
}

// init the arguments structure
t_args	*args_init(t_args *args, char **av)
{
	int			i;

	args->numbr_p = ft_itoa(av[1]);
	args->t2die_p = ft_itoa(av[2]);
	args->t2eat_p = ft_itoa(av[3]);
	args->t2slp_p = ft_itoa(av[4]);
	args->times_p = ft_itoa(av[5]);
	args->died = 0;
	args->fork = calloc(args->numbr_p, sizeof(int));
	if (!args->fork)
		return (NULL);
	args->fork_m = calloc(args->numbr_p, sizeof(pthread_mutex_t));
	if (!args->fork_m)
		return (free(args->fork), NULL);
	if (pthread_mutex_init(&args->print_mtx, NULL))
		return (free(args->fork), free(args->fork_m), NULL);
	if (pthread_mutex_init(&args->died_status, NULL))
	{
		pthread_mutex_destroy(&args->print_mtx);
		return (free(args->fork), free(args->fork_m), NULL);
	}
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
	i = -1;
	if (gettimeofday(&args->time, NULL) == -1)
	{
		while (++i < args->numbr_p)
			pthread_mutex_destroy(&args->fork_m[i]);
		pthread_mutex_destroy(&args->print_mtx);
		pthread_mutex_destroy(&args->died_status);
		return (free(args->fork), free(args->fork_m), NULL);
	}
	return (args);
}

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
int av_check(char **av)
{
	int i;
	int j;

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

int	main(int ac, char **av)
{
	t_args		args;
	int			i;
	t_philo		*philo;
	(void)		av;

	if (!(ac == 5 || ac == 6) || !av_check(av))
		return (printf("parameters: 4 or 5 positive numbers\n"), 2);
	if (!args_init(&args, av))
		return (1);
	if (args.numbr_p == 1)
	{
		printf("0 1 has taken the left fork\n");
		ft_msleep(args.t2die_p);
		printf("%i 1 has died\n", args.t2die_p);
		return (args_destroy(&args), 0);
	}
	philo = calloc(args.numbr_p, sizeof(t_philo));
	if (!philo)
		return (args_destroy(&args), 3);
	philo->args = &args;
	i = -1;
	while (++i < args.numbr_p)
	{
		philo[i].id = i;
		philo[i].args = &args;
		philo[i].tm_lmeal = args.time;
		if (pthread_create(&philo[i].thread_id, NULL, phl_thrd,
				(void *) &philo[i]))
			return (args_destroy(&args), free(philo), 4);
	}
	i = -1;
	while (++i < args.numbr_p)
		pthread_join(philo[i].thread_id, NULL);
	args_destroy (&args);
	free (philo);
	return (0);
}
