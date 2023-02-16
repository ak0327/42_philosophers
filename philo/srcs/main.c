/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/30 19:32:16 by takira            #+#    #+#             */
/*   Updated: 2023/02/16 16:13:04 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*start_each_philo(void *philo)
{
	// check for waiter

	// take fork & print

	// eat & print

	// release fork

	// sleep & print

	// think & print

}

int	check_philo_dead(t_args *args, t_philo *philo)
{
	size_t	idx;

	idx = 0;
	while (idx < args->num_of_philo)
	{

		idx++;
	}
}

int	start_spaghetti_party(t_args *args, t_philo *philo)
{
	size_t	idx;

	idx = 0;
	while (idx < args->num_of_philo)
	{
		if (pthread_create(&philo->philo_no[idx], NULL, start_each_philo, philo) != SUCCESS)
			return (PROCESS_ERROR);
		if (pthread_join(philo->philo_no[idx], NULL) != SUCCESS)
			return (PROCESS_ERROR);
		idx++;
	}
	while (true)
	{
		if (check_philo_dead(args, philo) == PHILO_DEAD)
			break ;
		usleep(CHECK_TIME_SPAM_MS); // check time span
	}
	return (SUCCESS);
}

void	free_allocs(t_args *args, t_philo *philo)
{
	free(args);
	if (!philo)
		return ;
	free(philo->philo_no);
	free(philo->forks);
	free(philo);
}

int	main(int argc, char **argv)
{
	t_args	*args;
	t_philo	*philo;

	if (!(5 <= argc && argc <= 6))
	{
		printf("[Error] Invalid argument. Input as following:\n" \
		"$>./args NumOfPhilos[1,200] TimeToDie[60,INT_MAX] " \
		"TimeToEat[60,INT_MAX] TimeToSleep[60,INT_MAX] (MustEatTimes[0,INT_MAX])\n");
		return (EXIT_FAILURE);
	}
	args = init_args();
	if (!args)
	{
		printf("[Error] Process error occurred\n");
		return (EXIT_FAILURE);
	}
	if (get_input_args(argv, args) == FAILURE)
		return (EXIT_FAILURE);

	printf("[#DEBUG]NumOfPhilos:%d, TimeToDie:%d, TimeToEat:%d, TimeToSleep:%d, MustEatTimes:%d\n", args->num_of_philo, args->time_to_die, args->time_to_eat, args->time_to_sleep, args->must_eat_times);

	philo = init_philo(args);
	if (!philo)
	{
		free(args);
		printf("[Error] Process error occurred\n");
		return (EXIT_FAILURE);
	}

	if (start_spaghetti_party(args, philo) == PROCESS_ERROR)
		return (EXIT_FAILURE);

	free_allocs(args, philo);
	return (EXIT_SUCCESS);
}
