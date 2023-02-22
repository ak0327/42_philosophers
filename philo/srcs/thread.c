/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_philo.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/15 19:50:32 by takira            #+#    #+#             */
/*   Updated: 2023/02/22 22:00:54 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	create_threads(t_params *params)
{
	struct timeval	tv;
	size_t			idx;

	gettimeofday(&tv, NULL);
	idx = 1;
	while (idx < params->num_of_philos)
	{
		params->philo_info[idx].start_time = tv;
		if (pthread_create(&params->philo_tid[idx], NULL, routine, \
		(void *)&params->philo_info[idx]) != SUCCESS)
			return (PROCESS_ERROR);
		idx += 2;
	}
	idx = 0;
	while (idx < params->num_of_philos)
	{
		params->philo_info[idx].start_time = tv;
		if (pthread_create(&params->philo_tid[idx], NULL, routine, \
		(void *)&params->philo_info[idx]) != SUCCESS)
			return (PROCESS_ERROR);
		idx += 2;
	}
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
	return (SUCCESS);
}
