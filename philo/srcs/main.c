/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/30 19:32:16 by takira            #+#    #+#             */
/*   Updated: 2023/02/09 19:07:45 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

t_args	*init_args(void)
{
	t_args	*args;

	args = (t_args *)malloc(sizeof(t_args));
	if (!args)
		return (NULL);
	memset(args, 0, sizeof(*args));

	return (args);
}

int	print_msg_ret_int(char *msg, int ret_num)
{
	if (msg)
		printf("%s\n", msg);
	return (ret_num);
}

int	check_num_valid(int num, t_input_type type)
{
	if (type == E_NUM_OF_PHILOS)
		return (1 <= num && num <= 200);
	if (type == E_TIME_TO_DIE)
		return (60 <= num);
	if (type == E_TIME_TO_EAT)
		return (60 <= num);
	if (type == E_TIME_TO_SLEEP)
		return (60 <= num);
	if (type == E_MUST_EAT_TIMES)
		return (0 <= num);
	return (0);
}

int	get_input_args(char **argv, t_args *args)
{
	bool	is_atoi_success;

	args->num_of_philo = ft_atoi(argv[NUM_OF_PHILOS_IDX], &is_atoi_success);
	if (!is_atoi_success && check_num_valid(args->num_of_philo, E_NUM_OF_PHILOS))
		return (print_msg_ret_int(ERROR_MSG_NUM_OF_PHILOS, FAILURE));//TODO: num_of_philo = 1
	args->time_to_die = ft_atoi(argv[TIME_TO_DIE_IDX], &is_atoi_success);
	if (!is_atoi_success && check_num_valid(args->time_to_die, E_TIME_TO_DIE))
		return (print_msg_ret_int(ERROR_MSG_TIME_TO_DIE, FAILURE));
	args->time_to_eat = ft_atoi(argv[TIME_TO_EAT_IDX], &is_atoi_success);
	if (!is_atoi_success && check_num_valid(args->time_to_eat, E_TIME_TO_EAT))
		return (print_msg_ret_int(ERROR_MSG_TIME_TO_EAT, FAILURE));
	args->time_to_sleep = ft_atoi(argv[TIME_TO_SLEEP_IDX], &is_atoi_success);
	if (!is_atoi_success && check_num_valid(args->time_to_sleep, E_TIME_TO_SLEEP))
		return (print_msg_ret_int(ERROR_MSG_TIME_TO_SLEEP, FAILURE));
	if (argv[MUST_EAT_TIMES_IDX])
	{
		args->must_eat_times = ft_atoi(argv[MUST_EAT_TIMES_IDX], &is_atoi_success);
		if (!is_atoi_success && check_num_valid(args->must_eat_times, E_MUST_EAT_TIMES))
			return (print_msg_ret_int(ERROR_MSG_MUST_EAT_TIMES, FAILURE));//TODO: must_eat_times = 0
	}
	return (SUCCESS);
}

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
