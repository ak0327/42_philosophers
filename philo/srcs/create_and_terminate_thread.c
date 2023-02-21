/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_philo.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/15 19:50:32 by takira            #+#    #+#             */
/*   Updated: 2023/02/21 18:46:03 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	create_threads(t_params *params)
{
	const time_t	start_time = get_unix_time_ms();
	size_t			idx;

	idx = 0;
	while (idx < params->num_of_philos)
	{
		params->philo_info[idx].start_time = start_time;
		if (pthread_create(&params->thread_id[idx], NULL, routine, (void *)&params->philo_info[idx]) != SUCCESS)
			return (PROCESS_ERROR);
		idx++;
	}
	if (pthread_create(&params->monitor, NULL, monitor, (void *)&params) != SUCCESS)
		return (PROCESS_ERROR);
	return (SUCCESS);
}

int	terminate_threads(t_params *params)
{
	size_t	idx;

	idx = 0;
	while (idx < params->num_of_philos)
	{
		params->philo_info[idx].is_continue = false;
		idx++;
	}
	idx = 0;
	while (idx < params->num_of_philos)
	{
		if (pthread_join(params->thread_id[idx], NULL) != SUCCESS)
			return (PROCESS_ERROR);
		idx++;
	}
	params->is_continue_monitor = false;
	if (pthread_join(params->monitor, NULL) != SUCCESS)
		return (PROCESS_ERROR);
	return (SUCCESS);
}

int	destroy_params(t_params *params)
{
	size_t	idx;

	idx = 0;
	while (idx < params->num_of_philos)
	{
		if (pthread_mutex_destroy(&params->fork_mutex[idx]) != SUCCESS)
			return (PROCESS_ERROR);
		idx++;
	}
	if (pthread_mutex_destroy(&params->print_mutex) != SUCCESS)
		return (PROCESS_ERROR);

	if (pthread_mutex_destroy(&params->died_mutex) != SUCCESS)
		return (PROCESS_ERROR);

	free_params(&params);

	return (SUCCESS);
}
