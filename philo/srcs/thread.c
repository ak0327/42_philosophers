/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_philo.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/15 19:50:32 by takira            #+#    #+#             */
/*   Updated: 2023/02/24 13:25:28 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	create_threads(t_params *params)
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

	idx = 1;
	print_msg(0, TYPE_SIM_START, params);
	while (idx < params->num_of_philos)
	{
		if (pthread_create(&params->philo_tid[idx], NULL, routine, \
		(void *)&params->philo_info[idx]) != SUCCESS)
			return (PROCESS_ERROR);
		idx += 2;
	}
	idx = 0;
	while (idx < params->num_of_philos)
	{
		if (pthread_create(&params->philo_tid[idx], NULL, routine, \
		(void *)&params->philo_info[idx]) != SUCCESS)
			return (PROCESS_ERROR);
		idx += 2;
	}
	params->start_time = start_time;
	if (pthread_create(&params->monitor_tid, NULL, monitor, \
		(void *)&params) != SUCCESS)
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
