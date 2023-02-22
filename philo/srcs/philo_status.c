/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_status_check.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/22 11:22:42 by takira            #+#    #+#             */
/*   Updated: 2023/02/22 20:35:07 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "philo.h"

static ssize_t	get_eat_times(t_philo_info *philo)
{
	size_t	eat_times;

	if (pthread_mutex_lock(&philo->philo_mutex) != SUCCESS)
		return (-1);
	eat_times = philo->eat_times;
	if (pthread_mutex_unlock(&philo->philo_mutex) != SUCCESS)
		return (-1);
	return ((ssize_t)eat_times);
}

int	is_meet_must_eat_times(t_params *params, int prev_ret_value)
{
	size_t	idx;
	size_t	arg_must_eat_times;
	ssize_t	philo_eat_times;

	if (prev_ret_value != SUCCESS)
		return (prev_ret_value);
	if (params->must_eat_times < 0)
		return (SUCCESS);
	arg_must_eat_times = (size_t)params->must_eat_times;
	idx = 0;
	while (idx < params->num_of_philos)
	{
		philo_eat_times = get_eat_times(&params->philo_info[idx]);
		if (philo_eat_times == -1)
			return (PROCESS_ERROR);
		if ((size_t)philo_eat_times < arg_must_eat_times)
			return (SUCCESS);
		idx++;
	}
	return (BREAK);
}

int	get_is_died(t_params *params, ssize_t *idx, int prev_ret_value)
{
	int	is_died;

	if (prev_ret_value != SUCCESS)
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

static struct timeval	get_start_time(t_philo_info *philo)
{
	struct timeval	start_time;

	pthread_mutex_lock(&philo->philo_mutex);
	start_time = philo->start_time;
	pthread_mutex_unlock(&philo->philo_mutex);
	return (start_time);
}

int	is_some_philo_died(t_params *params)
{
	struct timeval	now_tv;
	struct timeval	start_tv;
	time_t			delta_time;
	size_t			idx;
	int				is_died;

	idx = 0;
	if (get_is_died(params, NULL, SUCCESS) == PHILO_DIED)
		return (PHILO_DIED);
	if (pthread_mutex_lock(&params->died_mutex) != SUCCESS)
		return (PROCESS_ERROR);
	is_died = PHILO_ALIVE;
	gettimeofday(&now_tv, NULL);
	while (idx < params->num_of_philos)
	{
		start_tv = get_start_time(&params->philo_info[idx]);
		delta_time = (now_tv.tv_sec - start_tv.tv_sec) * 1000 \
		+ (now_tv.tv_usec - start_tv.tv_usec) / 1000;
		if (delta_time >= params->time_to_die)
		{
			params->is_died = PHILO_DIED;
			params->died_idx = (ssize_t)idx;
			print_msg(idx, TYPE_DIED, params, now_tv);
			params->died_idx = -1;
			is_died = PHILO_DIED;
			break ;
		}
		idx++;
	}
	if (pthread_mutex_unlock(&params->died_mutex) != SUCCESS)
		return (PROCESS_ERROR);
	return (is_died);
}
