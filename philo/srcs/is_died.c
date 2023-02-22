/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_died.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/22 23:11:00 by takira            #+#    #+#             */
/*   Updated: 2023/02/22 23:19:52 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static struct timeval	get_start_time(t_philo_info *philo)
{
	struct timeval	start_time;

	pthread_mutex_lock(&philo->philo_mutex);
	start_time = philo->start_time;
	pthread_mutex_unlock(&philo->philo_mutex);
	return (start_time);
}

static time_t	get_delta_time(struct timeval now_tv, struct timeval start_tv)
{
	return ((now_tv.tv_sec - start_tv.tv_sec) * 1000 \
		+ (now_tv.tv_usec - start_tv.tv_usec) / 1000);
}

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

int	is_some_philo_died(t_params *params)
{
	struct timeval	now_tv;
	struct timeval	start_tv;
	size_t			idx;

	idx = 0;
	if (get_is_died(params, NULL, SUCCESS) == PHILO_DIED)
		return (PHILO_DIED);
	gettimeofday(&now_tv, NULL);
	while (idx < params->num_of_philos)
	{
		start_tv = get_start_time(&params->philo_info[idx]);
		if (!get_is_meet_eat_times(&params->philo_info[idx]) \
		&& get_delta_time(now_tv, start_tv) >= params->time_to_die)
		{
			if (update_died(params, (ssize_t)idx) == PROCESS_ERROR)
				return (PROCESS_ERROR);
			print_msg(idx, TYPE_DIED, params);
			if (update_died(params, -1) == PROCESS_ERROR)
				return (PROCESS_ERROR);
			return (PHILO_DIED);
		}
		idx++;
	}
	return (PHILO_ALIVE);
}
