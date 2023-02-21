/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_philo.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/15 19:50:32 by takira            #+#    #+#             */
/*   Updated: 2023/02/21 14:47:10 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	create_threads(t_params *params)
{
	const time_t	start_time = get_unix_time_ms();
	size_t			*philo_id;
	pthread_t		*tid;
	size_t			idx;

	idx = 0;
	while (idx < params->num_of_philos)
	{
		params->each_start_time[idx] = start_time;
		tid = &params->tid[idx];
		philo_id = &params->philo_idx[idx];
		if (pthread_create(tid, NULL, routine, (void *)philo_id) != SUCCESS)
			return (PROCESS_ERROR);
		idx++;
	}
	return (SUCCESS);
}

int	terminate_threads(t_params *params)
{
	size_t	idx;

	idx = 0;
	while (idx < params->num_of_philos)
	{
		if (pthread_join(params->tid[idx], NULL) != SUCCESS)
			return (PROCESS_ERROR);
		idx++;
	}
	return (SUCCESS);
}
