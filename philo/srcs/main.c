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

void	*start_each_philo(void *v_param)
{
	t_params	*params;

	params = (t_params *)v_param;

	// lock waiter and check
	pthread_mutex_lock(&params->waiter);

	// take fork & print


	// unlock waiter
	pthread_mutex_unlock(&params->waiter);

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
		if (pthread_create(&philo->no[idx], NULL, start_each_philo, philo) != SUCCESS)
			return (PROCESS_ERROR);
		if (pthread_join(philo->no[idx], NULL) != SUCCESS)
			return (PROCESS_ERROR);
		idx++;
	}
	while (true)
	{
		if (check_philo_dead(args, philo) == PHILO_DEAD)
			break ;
		usleep(CHECK_TIME_SPAN_MS); // check time span
	}
	return (SUCCESS);
}

void	free_allocs(t_params *params)
{
	if (!params)
		return ;
	free(params->philo_no);
	free(params->forks);
	free(params);
}

int	main(int argc, char **argv)
{
	t_params	*params;
	int			ret_value;

	ret_value = init_params(argc, argv, params);
	if (ret_value != SUCCESS)
	{
		print_error(ret_value);
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

	printf("[#DEBUG]NumOfPhilos:%d, TimeToDie:%d, TimeToEat:%d, TimeToSleep:%d, MustEatTimes:%d\n", args->num_of_philos, args->time_to_die, args->time_to_eat, args->time_to_sleep, args->must_eat_times);

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
