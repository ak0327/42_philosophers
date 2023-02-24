/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_died.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/22 23:11:00 by takira            #+#    #+#             */
/*   Updated: 2023/02/24 13:43:08 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static  int	check_and_update_died_wo_lock(\
t_params *params, size_t idx, time_t now_time)
{
	time_t	start_time;
	time_t	delta_time;
	bool	is_meet_eat_times;

	is_meet_eat_times = get_is_meet_eat_times(&params->philo_info[idx]);
	start_time = get_start_time(&params->philo_info[idx]);
	delta_time = get_delta_time(now_time, start_time);
	if (!is_meet_eat_times && delta_time >= params->time_to_die)
	{
		params->is_died = PHILO_DIED;
		params->died_idx = (ssize_t)idx;
//		print_msg(idx, TYPE_DIED, params);
//		params->died_idx = -1;
		return (PHILO_DIED);
	}
	return (PHILO_ALIVE);
}

int	check_and_update_died(t_params *params, size_t idx, time_t now_time)
{
	int		ret_value;
	int		is_died;

	if (pthread_mutex_lock(&params->died_mutex) != SUCCESS)
		return (PROCESS_ERROR);
	is_died = params->is_died;
	if (is_died == PHILO_ALIVE)
		ret_value = check_and_update_died_wo_lock(params, idx, now_time);
	if (pthread_mutex_unlock(&params->died_mutex) != SUCCESS)
		return (PROCESS_ERROR);
	return (ret_value);
}

int	is_some_philo_died(t_params *params)
{
	const time_t	now_time = get_unix_time_ms();
	size_t			idx;
	int				ret_val;

	idx = 0;
	if (pthread_mutex_lock(&params->died_mutex) != SUCCESS)
		return (PROCESS_ERROR);
	ret_val = params->is_died;
	while (ret_val == PHILO_ALIVE && idx < params->num_of_philos)
	{
		ret_val = check_and_update_died_wo_lock(params, idx, now_time);
		if (ret_val != SUCCESS)
			break ;
		idx++;
	}
	if (pthread_mutex_unlock(&params->died_mutex) != SUCCESS)
		return (PROCESS_ERROR);
	return (ret_val);
}

int	get_is_died(t_params *params, ssize_t *idx, int prev_ret_value)
{
	int	is_died;

	if (!(prev_ret_value == SUCCESS || prev_ret_value == CONTINUE))
		return (prev_ret_value);
	if (pthread_mutex_lock(&params->died_mutex) != SUCCESS)
		return (PROCESS_ERROR);
	is_died = params->is_died;
	if (idx)
		*idx = params->died_idx;
	if (pthread_mutex_unlock(&params->died_mutex) != SUCCESS)
		return (PROCESS_ERROR);
	return (is_died);
}
