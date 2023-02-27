/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_philo.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/15 19:50:32 by takira            #+#    #+#             */
/*   Updated: 2023/02/27 14:39:14 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	set_start_time(t_params *params)
{
	time_t	start_time;
	size_t	idx;

	start_time = get_unix_time_ms();
	idx = 0;
	while (idx < params->num_of_philos)
	{
		params->philo_info[idx].start_time = start_time;
		idx++;
	}
	params->start_time = start_time;
}

static int	create_philo_threads(t_params *params)
{
	size_t	idx;

	idx = 1;
	print_msg(0, TYPE_SIM_START, params);
	while (idx < params->num_of_philos)
	{
		if (pthread_create(&params->philo_tid[idx], NULL, routine, \
		(void *)&params->philo_info[idx]) != SUCCESS)
			return (PROCESS_ERROR);
		idx += 2;
	}
	usleep(10);
	idx = 0;
	while (idx < params->num_of_philos)
	{
		if (pthread_create(&params->philo_tid[idx], NULL, routine, \
		(void *)&params->philo_info[idx]) != SUCCESS)
			return (PROCESS_ERROR);
		idx += 2;
	}
	return (SUCCESS);
}

static int	create_monitor_thread(t_params *params)
{
	if (pthread_create(&params->monitor_tid, NULL, monitor, \
		(void *)params) != SUCCESS)
		return (PROCESS_ERROR);
	return (SUCCESS);
}

int	create_threads(t_params *params)
{
	set_start_time(params);
	if (create_philo_threads(params) == PROCESS_ERROR)
		return (PROCESS_ERROR);
	if (create_monitor_thread(params) == PROCESS_ERROR)
		return (PROCESS_ERROR);
	return (SUCCESS);
}

int	join_threads(t_params *params)
{
	size_t	idx;

	idx = 0;
	while (idx < params->num_of_philos)
	{
		if (pthread_join(params->philo_tid[idx], NULL) != SUCCESS)
			return (PROCESS_ERROR);
		idx++;
	}
	if (pthread_join(params->monitor_tid, NULL) != SUCCESS)
		return (PROCESS_ERROR);
	return (SUCCESS);
}
