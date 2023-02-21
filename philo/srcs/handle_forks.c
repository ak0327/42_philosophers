/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_forks.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/20 09:56:44 by takira            #+#    #+#             */
/*   Updated: 2023/02/21 21:42:31 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	take_forks(t_philo_info *philo)
{
	const size_t	first_take = philo->first_take;
	const size_t	second_take = philo->second_take;
	t_params		*params;

	params = philo->params_ptr;

//	if (params->prev_used_by[first_take] == (ssize_t)philo->idx)
//	{
//		params->held_by[first_take] = -1;
//		params->prev_used_by[first_take] = -1;
//	}
	pthread_mutex_lock(&params->fork_mutex[first_take]);
	params->held_by[first_take] = (ssize_t)philo->idx;
	print_msg(philo->idx, TYPE_FORK, get_unix_time_ms(), params);

	pthread_mutex_lock(&params->fork_mutex[second_take]);
	params->held_by[second_take] = (ssize_t)philo->idx;
	print_msg(philo->idx, TYPE_FORK, get_unix_time_ms(), params);

//	if (params->held_by[first_take] != (ssize_t)philo->idx)
//	{
//		pthread_mutex_lock(&params->fork_mutex[first_take]);
//		params->held_by[first_take] = (ssize_t)philo->idx;
//	}
	return (SUCCESS);
}

int	put_forks(t_philo_info *philo)
{
	const size_t	first_take = philo->first_take;
	const size_t	second_take = philo->second_take;
	t_params		*params;

	params = philo->params_ptr;

	params->held_by[first_take] = -1;
	params->prev_used_by[first_take] = (ssize_t)philo->idx;
	pthread_mutex_unlock(&params->fork_mutex[first_take]);
	params->held_by[second_take] = -1;
	params->prev_used_by[second_take] = (ssize_t)philo->idx;
	pthread_mutex_unlock(&params->fork_mutex[second_take]);

	return (SUCCESS);
}

