/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/30 19:32:16 by takira            #+#    #+#             */
/*   Updated: 2023/02/09 17:59:20 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

t_info	*init_info(void)
{
	t_info	*info;

	info = (t_info *)malloc(sizeof(t_info));
	if (!info)
		return (NULL);
	info->num_of_philo = 0;
	info->time_to_die = 0;
	info->time_to_eat = 0;
	info->time_to_sleep = 0;
	info->must_eat_times = 0;
	return (info);
}

int	msg_and_return_num(char *msg, int ret_num)
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

int	get_input(int argc, char **argv, t_info *info)
{
	bool	is_atoi_success;

	info->num_of_philo = ft_atoi(argv[NUM_OF_PHILOS_IDX], &is_atoi_success);
	if (!is_atoi_success && check_num_valid(info->num_of_philo, E_NUM_OF_PHILOS))
		return (msg_and_return_num(ERROR_MSG_NUM_OF_PHILOS, FAIL));//TODO: num_of_philo = 1
	info->time_to_die = ft_atoi(argv[TIME_TO_DIE_IDX], &is_atoi_success);
	if (!is_atoi_success && check_num_valid(info->time_to_die, E_TIME_TO_DIE))
		return (msg_and_return_num(ERROR_MSG_TIME_TO_DIE, FAIL));
	info->time_to_eat = ft_atoi(argv[TIME_TO_EAT_IDX], &is_atoi_success);
	if (!is_atoi_success && check_num_valid(info->time_to_eat, E_TIME_TO_EAT))
		return (msg_and_return_num(ERROR_MSG_TIME_TO_EAT, FAIL));
	info->time_to_sleep = ft_atoi(argv[TIME_TO_SLEEP_IDX], &is_atoi_success);
	if (!is_atoi_success && check_num_valid(info->time_to_sleep, E_TIME_TO_SLEEP))
		return (msg_and_return_num(ERROR_MSG_TIME_TO_SLEEP, FAIL));
	if (argc == MUST_EAT_TIMES_IDX)
	{
		info->must_eat_times = ft_atoi(argv[MUST_EAT_TIMES_IDX], &is_atoi_success);
		if (!is_atoi_success && check_num_valid(info->must_eat_times, E_MUST_EAT_TIMES))
			return (msg_and_return_num(ERROR_MSG_MUST_EAT_TIMES, FAIL));//TODO: must_eat_times = 0
	}
	return (PASS);
}

int	main(int argc, char **argv)
{
	t_info	*info;

	if (argc < 5 || 6 < argc)
	{
		printf("[Error] Invalid argument. Input as following:\n" \
		"$>./philo NumOfPhilos(1~200) TimeToDie(60~) TimeToEat(60~) TimeToSleep(60~) <MustEatTimes(0~)>\n");
		return (EXIT_FAILURE);
	}
	info = init_info();
	if (!info)
	{
		printf("[Error] Fail to malloc\n");
		return (EXIT_FAILURE);
	}
	if (get_input(argc, argv, info) == FAIL)
		return (EXIT_FAILURE);
	printf("NumOfPhilos:%d, TimeToDie:%d, TimeToEat:%d, TimeToSleep:%d, MustEatTimes:%d\n", info->num_of_philo, info->time_to_die, info->time_to_eat, info->time_to_sleep, info->must_eat_times);

	return (0);
}
