/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_args.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/15 19:27:11 by takira            #+#    #+#             */
/*   Updated: 2023/02/17 11:03:08 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	print_msg_ret_int(const char *msg, int ret_num)
{
	if (msg)
		printf("%s\n", msg);
	return (ret_num);
}

static int	check_num_validity(long long num, t_input_type type)
{
	if (type == TYPE_NUM_OF_PHILO)
		return (1 <= num && num <= 200);
	if (type == TYPE_TIME_TO_DIE)
		return (60 <= num);
	if (type == TYPE_TIME_TO_EAT)
		return (60 <= num);
	if (type == TYPE_TIME_TO_SLEEP)
		return (60 <= num);
	return (0 <= num);
}

int	get_input_args(char **argv, t_params *params)
{
	bool		is_success;
	long long	arg_num;

	arg_num = ft_strtoll(argv[NUM_OF_PHILOS_IDX], &is_success);
	if (!is_success && check_num_validity(arg_num, TYPE_NUM_OF_PHILO))
		return (print_msg_ret_int(ERROR_MSG_NUM_OF_PHILOS, FAILURE)); //TODO: if num_of_philo = 1
	params->num_of_philo = (size_t)arg_num;

	arg_num = ft_strtoll(argv[TIME_TO_DIE_IDX], &is_success);
	if (!is_success && check_num_validity(arg_num, TYPE_TIME_TO_DIE))
		return (print_msg_ret_int(ERROR_MSG_TIME_TO_DIE, FAILURE));
	params->time_to_die = (time_t)arg_num;

	arg_num = ft_strtoll(argv[TIME_TO_EAT_IDX], &is_success);
	if (!is_success && check_num_validity(arg_num, TYPE_TIME_TO_EAT))
		return (print_msg_ret_int(ERROR_MSG_TIME_TO_EAT, FAILURE));
	params->time_to_eat = (time_t)arg_num;

	arg_num = ft_strtoll(argv[TIME_TO_SLEEP_IDX], &is_success);
	if (!is_success && check_num_validity(arg_num, TYPE_TIME_TO_SLEEP))
		return (print_msg_ret_int(ERROR_MSG_TIME_TO_SLEEP, FAILURE));
	params->time_to_sleep = (time_t)arg_num;

	params->must_eat_times = -1;
	if (argv[MUST_EAT_TIMES_IDX])
	{
		arg_num = ft_strtoll(argv[MUST_EAT_TIMES_IDX], &is_success);
		if (!is_success && check_num_validity(arg_num, TYPE_MUST_EAT_TIMES))
			return (print_msg_ret_int(ERROR_MSG_MUST_EAT_TIMES, FAILURE)); //TODO: if must_eat_times = 0
		params->must_eat_times = arg_num;
	}
	return (SUCCESS);
}
