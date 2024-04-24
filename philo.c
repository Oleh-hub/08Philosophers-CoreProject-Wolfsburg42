/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oruban <oruban@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/29 15:14:18 by oruban            #+#    #+#             */
/*   Updated: 2024/04/24 21:08:10 by oruban           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// https://nafuka11.github.io/philosophers-visualizer/
// recomended border cases to check
// ./philo 1 1010 500 500
// ./philo 4 190 200 100
// ./philo 4 290 200 100
// ./philo 4 310 200 100 //
// ./philo 4 390 200 100 //
// ./philo 5 490 200 100
// ./philo 5 590 200 100
// The following border cases fails by other students projects like in 115XXX
// ms, but not im my case:
// ./philo 200 1010 500 500 // no death evan after 355XXX ms...
// The tests with following border parameters failed by every other successful 
// student project I have seen in my case as well:
// ./philo 200 410 200 200 // fails when run long enough 20465 45XXX 3817 ms...
// ./philo 200 210 100 100 // fails when run long enough 6563 4145 3438 ms...

#include "philo.h"

// mutexed print out the philosopher's actions
static void	ft_printf_out(t_philo *philo, char *str)
{
	pthread_mutex_lock(&(philo->args->print_mtx));
	printf("%ld %d %s\n", get_time(philo->args->time), philo->id + 1, str);
	pthread_mutex_unlock(&philo->args->print_mtx);
}

// check if the philosopher is alive
int	is_alive(t_philo *philo)
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
// E.g.:	pthread_mutex_lock(&philo->args->fork_m[philo->id]);
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

////Thobias' idea
typedef void	*(*t_threadfun)(void *);

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
	{
		printf("0 1 has taken the left fork\n");
		ft_msleep(args.t2die_p);
		printf("%i 1 has died\n", args.t2die_p);
		return (args_destroy(&args), 0);
	}
	philo = ft_calloc(args.numbr_p, sizeof(t_philo));
	if (!philo)
		return (args_destroy(&args), 3);
	philo->args = &args;
	i = -1;
	while (++i < args.numbr_p)
	{
		philo[i].id = i;
		philo[i].args = &args;
		philo[i].tm_lmeal = args.time;
		if (pthread_create(&philo[i].thread_id, NULL, (t_threadfun) phl_thrd,
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
