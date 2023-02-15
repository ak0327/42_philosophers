/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_args.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/15 19:27:11 by takira            #+#    #+#             */
/*   Updated: 2023/02/15 19:37:58 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	print_msg_ret_int(const char *msg, int ret_num)
{
	if (msg)
		printf("%s\n", msg);
	return (ret_num);
}

static int	check_num_valid(int num, t_input_type type)
{
	if (type == TYPE_NUM_OF_PHILOS)
		return (1 <= num && num <= 200);
	if (type == TYPE_TIME_TO_DIE)
		return (60 <= num);
	if (type == TYPE_TIME_TO_EAT)
		return (60 <= num);
	if (type == TYPE_TIME_TO_SLEEP)
		return (60 <= num);
	if (type == TYPE_MUST_EAT_TIMES)
		return (0 <= num);
	return (0);
}

int	get_input_args(char **argv, t_args *args)
{
	bool	is_atoi_success;

	args->num_of_philo = ft_atoi(argv[NUM_OF_PHILOS_IDX], &is_atoi_success);
	if (!is_atoi_success && check_num_valid(args->num_of_philo, TYPE_NUM_OF_PHILOS))
		return (print_msg_ret_int(ERROR_MSG_NUM_OF_PHILOS, FAILURE));//TODO: num_of_philo = 1
	args->time_to_die = ft_atoi(argv[TIME_TO_DIE_IDX], &is_atoi_success);
	if (!is_atoi_success && check_num_valid(args->time_to_die, TYPE_TIME_TO_DIE))
		return (print_msg_ret_int(ERROR_MSG_TIME_TO_DIE, FAILURE));
	args->time_to_eat = ft_atoi(argv[TIME_TO_EAT_IDX], &is_atoi_success);
	if (!is_atoi_success && check_num_valid(args->time_to_eat, TYPE_TIME_TO_EAT))
		return (print_msg_ret_int(ERROR_MSG_TIME_TO_EAT, FAILURE));
	args->time_to_sleep = ft_atoi(argv[TIME_TO_SLEEP_IDX], &is_atoi_success);
	if (!is_atoi_success && check_num_valid(args->time_to_sleep, TYPE_TIME_TO_SLEEP))
		return (print_msg_ret_int(ERROR_MSG_TIME_TO_SLEEP, FAILURE));
	if (argv[MUST_EAT_TIMES_IDX])
	{
		args->must_eat_times = ft_atoi(argv[MUST_EAT_TIMES_IDX], &is_atoi_success);
		if (!is_atoi_success && check_num_valid(args->must_eat_times, TYPE_MUST_EAT_TIMES))
			return (print_msg_ret_int(ERROR_MSG_MUST_EAT_TIMES, FAILURE));//TODO: must_eat_times = 0
	}
	return (SUCCESS);
}

t_args	*init_args(void)
{
	t_args	*args;

	args = (t_args *)malloc(sizeof(t_args));
	if (!args)
		return (NULL);
	args->num_of_philo = 0;
	args->time_to_die = 0;
	args->time_to_eat = 0;
	args->time_to_sleep = 0;
	args->must_eat_times = -1;
	return (args);
}
