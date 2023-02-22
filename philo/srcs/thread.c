/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_philo.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/15 19:50:32 by takira            #+#    #+#             */
/*   Updated: 2023/02/22 20:29:04 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	create_threads(t_params *params)
{
	struct timeval	tv;
	size_t			idx;

	/*
	idx = 0;
	while (idx < params->num_of_philos)
	{
		params->philo_info[idx].start_time = start_time;
		if (pthread_create(&params->philo_tid[idx], NULL, routine, (void *)&params->philo_info[idx]) != SUCCESS)
			return (PROCESS_ERROR);
//		usleep(500);
		idx++;
	}
	 */

	gettimeofday(&tv, NULL);
	idx = 1;
	while (idx < params->num_of_philos)
	{
		params->philo_info[idx].start_time = tv;
		if (pthread_create(&params->philo_tid[idx], NULL, routine, (void *)&params->philo_info[idx]) != SUCCESS)
			return (PROCESS_ERROR);
		idx += 2;
	}
	idx = 0;
	while (idx < params->num_of_philos)
	{
		params->philo_info[idx].start_time = tv;
		if (pthread_create(&params->philo_tid[idx], NULL, routine, (void *)&params->philo_info[idx]) != SUCCESS)
			return (PROCESS_ERROR);
		idx += 2;
	}

	//	if (pthread_create(&params->monitor_tid, NULL, monitor, (void *)params) != SUCCESS)
//		return (PROCESS_ERROR);
	return (SUCCESS);
}

int	terminate_threads(t_params *params)
{
	size_t	idx;

	idx = 0;
	while (idx < params->num_of_philos)
	{
		pthread_mutex_lock(&params->philo_info[idx].philo_mutex);
		params->philo_info[idx].is_meet_eat_times = true;
		pthread_mutex_unlock(&params->philo_info[idx].philo_mutex);
		idx++;
	}
	idx = 0;
	while (idx < params->num_of_philos)
	{
		if (pthread_join(params->philo_tid[idx], NULL) != SUCCESS)
			return (PROCESS_ERROR);
		idx++;
	}
//	params->is_continue_monitor = false;
//	if (pthread_join(params->monitor_tid, NULL) != SUCCESS)
//		return (PROCESS_ERROR);
	return (SUCCESS);
}
