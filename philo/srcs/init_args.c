/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_args.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/15 19:27:11 by takira            #+#    #+#             */
/*   Updated: 2023/02/22 21:04:38 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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
	return (1 <= num);
}

int	get_input_args(char **argv, t_params *params)
{
	bool		is_success;
	long long	arg_num;

	arg_num = ft_strtoll(argv[NUM_OF_PHILOS_IDX], &is_success);
	if (!is_success && check_num_validity(arg_num, TYPE_NUM_OF_PHILO))
		return (INVALID_NUM_OF_PHILOS); //TODO: if num_of_philos = 1
	params->num_of_philos = (size_t)arg_num;

	arg_num = ft_strtoll(argv[TIME_TO_DIE_IDX], &is_success);
	if (!is_success && check_num_validity(arg_num, TYPE_TIME_TO_DIE))
		return (INVALID_TIME_TO_DIE);
	params->time_to_die = (time_t)arg_num;

	arg_num = ft_strtoll(argv[TIME_TO_EAT_IDX], &is_success);
	if (!is_success && check_num_validity(arg_num, TYPE_TIME_TO_EAT))
		return (INVALID_TIME_TO_EAT);
	params->time_to_eat = (time_t)arg_num;

	arg_num = ft_strtoll(argv[TIME_TO_SLEEP_IDX], &is_success);
	if (!is_success && check_num_validity(arg_num, TYPE_TIME_TO_SLEEP))
		return (INVALID_TIME_TO_SLEEP);
	params->time_to_sleep = (time_t)arg_num;

	params->must_eat_times = -1;
	if (argv[MUST_EAT_TIMES_IDX])
	{
		arg_num = ft_strtoll(argv[MUST_EAT_TIMES_IDX], &is_success);
		if (!is_success && check_num_validity(arg_num, TYPE_MUST_EAT_TIMES))
			return (INVALID_MUST_EAT_TIMES);
		params->must_eat_times = (ssize_t)arg_num;
	}
	return (SUCCESS);
}
