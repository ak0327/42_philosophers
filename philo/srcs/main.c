/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/30 19:32:16 by takira            #+#    #+#             */
/*   Updated: 2023/02/15 19:39:18 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

t_philo	*init_philo(t_args *args)
{
	t_philo	*philo;

	if (!args)
		return (NULL);


	return (philo);
}

int	activate_philo(t_args *args, t_philo *philo)
{

}

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
		if (pthread_create(&philo->idx[idx], NULL, start_each_philo, philo) != SUCCESS)
			return (PROCESS_ERROR);
		if (pthread_join(philo->idx[idx], NULL) != SUCCESS)
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

void	free_allocs(t_args *args)
{

}

int	main(int argc, char **argv)
{
	t_args	*args;
	t_philo	*philo;

	if (argc < 5 || 6 < argc)
	{
		printf("[Error] Invalid argument. Input as following:\n" \
		"$>./args NumOfPhilos(1~200) TimeToDie(60~) " \
		"TimeToEat(60~) TimeToSleep(60~) <OP>MustEatTimes(0~)\n");
		return (EXIT_FAILURE);
	}
	args = init_args();
	if (!args)
	{
		printf("[Error] Fail to malloc\n");
		return (EXIT_FAILURE);
	}
	if (get_input_args(argv, args) == FAILURE)
		return (EXIT_FAILURE);
	printf("[#DEBUG]NumOfPhilos:%d, TimeToDie:%d, TimeToEat:%d, TimeToSleep:%d, MustEatTimes:%d\n", args->num_of_philo, args->time_to_die, args->time_to_eat, args->time_to_sleep, args->must_eat_times);

	philo = init_philo(args);
	if (!philo)
	{
		printf("[Error] Fail to malloc\n");
		return (EXIT_FAILURE);
	}
	if (start_spaghetti_party(args, philo) == PROCESS_ERROR)
		return (EXIT_FAILURE);
	free_allocs(args);
	return (EXIT_SUCCESS);
}
