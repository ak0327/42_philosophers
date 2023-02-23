/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_died.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/22 23:11:00 by takira            #+#    #+#             */
/*   Updated: 2023/02/23 15:47:23 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
/*
static int	update_died(t_params *params, ssize_t idx)
{
	if (pthread_mutex_lock(&params->died_mutex) != SUCCESS)
		return (PROCESS_ERROR);
	params->is_died = PHILO_DIED;
	params->died_idx = idx;
	if (pthread_mutex_unlock(&params->died_mutex) != SUCCESS)
		return (PROCESS_ERROR);
	return (SUCCESS);
}
*/

int	check_and_update_died_w_lock(t_params *params, size_t idx, t_timeval now_tv)
{
	t_timeval	start_tv;
	time_t		delta_time;
	bool		is_meet_eat_times;
	int			ret_value;

	is_meet_eat_times = get_is_meet_eat_times(&params->philo_info[idx]);
	if (!is_meet_eat_times)
		return (PHILO_ALIVE);
	if (pthread_mutex_lock(&params->died_mutex) != SUCCESS)
		return (PROCESS_ERROR);
	ret_value = PHILO_ALIVE;
	start_tv = get_start_time(&params->philo_info[idx]);
	delta_time = get_delta_time(now_tv, start_tv);
	if (params->is_died == PHILO_ALIVE && delta_time >= params->time_to_die)
	{
		params->is_died = PHILO_DIED;
		params->died_idx = (ssize_t)idx;
		print_msg(idx, TYPE_DIED, params);
		params->died_idx = -1;
		ret_value = PHILO_DIED;
	}
	if (pthread_mutex_unlock(&params->died_mutex) != SUCCESS)
		return (PROCESS_ERROR);
	return (ret_value);
}


int	check_and_update_died_wo_lock(t_params *params, size_t idx, t_timeval now_tv)
{
	t_timeval	start_tv;
	time_t		delta_time;
	bool		is_meet_eat_times;

	is_meet_eat_times = get_is_meet_eat_times(&params->philo_info[idx]);
	start_tv = get_start_time(&params->philo_info[idx]);
	delta_time = get_delta_time(now_tv, start_tv);
	if (!is_meet_eat_times && delta_time >= params->time_to_die)
	{

//		if (pthread_mutex_lock(&params->died_mutex) != SUCCESS)
//			return (PROCESS_ERROR);
		params->is_died = PHILO_DIED;
		params->died_idx = (ssize_t)idx;
//		if (pthread_mutex_unlock(&params->died_mutex) != SUCCESS)
//			return (PROCESS_ERROR);
		print_msg(idx, TYPE_DIED, params);
		params->died_idx = -1;
		return (PHILO_DIED);
		/*
		if (update_died(params, (ssize_t)idx) == PROCESS_ERROR)
			return (PROCESS_ERROR);
		print_msg(idx, TYPE_DIED, params);
		if (update_died(params, -1) == PROCESS_ERROR)
			return (PROCESS_ERROR);
		return (PHILO_DIED);
		 */
	}
	return (PHILO_ALIVE);
}

int	is_some_philo_died(t_params *params)
{
	t_timeval	now_tv;
	size_t		idx;
	int			ret_val;

	idx = 0;
//	if (get_is_died(params, NULL, SUCCESS) == PHILO_DIED)
//		return (PHILO_DIED);
	if (pthread_mutex_lock(&params->died_mutex) != SUCCESS)
		return (PROCESS_ERROR);
	ret_val = params->is_died;
	gettimeofday(&now_tv, NULL);
	while (ret_val == PHILO_ALIVE && idx < params->num_of_philos)
	{
		ret_val = check_and_update_died_wo_lock(params, idx, now_tv);
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
