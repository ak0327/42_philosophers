/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_forks.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/20 09:56:44 by takira            #+#    #+#             */
/*   Updated: 2023/02/22 19:00:36 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static ssize_t	get_prev_used_by(size_t fork_idx, t_params *params)
{
	ssize_t	prev_used_by;

	pthread_mutex_lock(&params->prev_used_mutex[fork_idx]);
	prev_used_by = params->prev_used_by[fork_idx];
	pthread_mutex_unlock(&params->prev_used_mutex[fork_idx]);
	return (prev_used_by);
}

static int	update_prev_used_by(size_t fork_idx, size_t used_by, t_params *params)
{
	if (pthread_mutex_lock(&params->prev_used_mutex[fork_idx]) != SUCCESS)
		return (PROCESS_ERROR);
	params->prev_used_by[fork_idx] = (ssize_t)used_by;
	if (pthread_mutex_unlock(&params->prev_used_mutex[fork_idx]) != SUCCESS)
		return (PROCESS_ERROR);
	return (SUCCESS);
}

int	take_forks(t_philo_info *philo)
{
	t_params	*params;
	ssize_t		first_prev;
	ssize_t		second_prev;

	params = philo->params_ptr;
	first_prev = get_prev_used_by(philo->first_take, params);
	second_prev = get_prev_used_by(philo->second_take, params);
	if (first_prev == (ssize_t)philo->idx || second_prev == (ssize_t)philo->idx)
		return (FAILURE);
	if (pthread_mutex_lock(&params->fork_mutex[philo->first_take]) != SUCCESS)
		return (PROCESS_ERROR);
//	params->held_by[philo->first_take] = (ssize_t)idx;
	if (print_msg(philo->idx, TYPE_FORK, params, get_unix_time_ms()) == FAILURE)
	{
		pthread_mutex_unlock(&params->fork_mutex[philo->first_take]);
		return (FAILURE);
	}
	if (pthread_mutex_lock(&params->fork_mutex[philo->second_take]) != SUCCESS)
		return (PROCESS_ERROR);
//	params->held_by[philo->second_take] = (ssize_t)philo->idx;
	if (print_msg(philo->idx, TYPE_FORK, params, get_unix_time_ms()) == FAILURE)
	{
		pthread_mutex_unlock(&params->fork_mutex[philo->first_take]);
		pthread_mutex_unlock(&params->fork_mutex[philo->second_take]);
		return (FAILURE);
	}
	return (SUCCESS);
}

int	put_forks(t_philo_info *philo, int prev_ret_val)
{
	t_params	*params;
	size_t		first_take;
	size_t		second_take;

	if (prev_ret_val != SUCCESS)
		return (prev_ret_val);

	params = philo->params_ptr;
	first_take = philo->first_take;
	second_take = philo->second_take;

//	params->held_by[first_take] = -1;
	if (update_prev_used_by(first_take, philo->idx, params) != SUCCESS)
		return (PROCESS_ERROR);
//	params->prev_used_by[first_take] = (ssize_t)philo->idx;
	if (pthread_mutex_unlock(&params->fork_mutex[first_take]) != SUCCESS)
		return (PROCESS_ERROR);

//	params->held_by[second_take] = -1;
//	params->prev_used_by[second_take] = (ssize_t)philo->idx;
	if (update_prev_used_by(second_take, philo->idx, params) != SUCCESS)
		return (PROCESS_ERROR);
	if (pthread_mutex_unlock(&params->fork_mutex[second_take]) != SUCCESS)
		return (PROCESS_ERROR);

	return (SUCCESS);
}

