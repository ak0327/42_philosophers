/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_forks.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/20 09:56:44 by takira            #+#    #+#             */
/*   Updated: 2023/02/22 17:41:44 by takira           ###   ########.fr       */
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

static void	update_prev_used_by(size_t fork_idx, size_t used_by, t_params *params)
{
	pthread_mutex_lock(&params->prev_used_mutex[fork_idx]);
	params->prev_used_by[fork_idx] = (ssize_t)used_by;
	pthread_mutex_unlock(&params->prev_used_mutex[fork_idx]);
}

int	take_forks(t_philo_info *philo)
{
	t_params	*params;
	size_t		first_take;
	size_t		second_take;
	ssize_t		first_prev;
	ssize_t		second_prev;

	params = philo->params_ptr;
	first_take = philo->first_take;
	second_take = philo->second_take;

	first_prev = get_prev_used_by(first_take, params);
	second_prev = get_prev_used_by(second_take, params);

	if (first_prev == (ssize_t)philo->idx || second_prev == (ssize_t)philo->idx)
		return (FAILURE);

	pthread_mutex_lock(&params->fork_mutex[first_take]);
//	params->held_by[first_take] = (ssize_t)idx;
	print_msg(philo->idx, TYPE_FORK, params);

	pthread_mutex_lock(&params->fork_mutex[second_take]);
//	params->held_by[second_take] = (ssize_t)philo->idx;
	print_msg(philo->idx, TYPE_FORK, params);
	return (SUCCESS);
}

int	put_forks(t_philo_info *philo)
{
	t_params	*params;
	size_t		first_take;
	size_t		second_take;

	params = philo->params_ptr;
	first_take = philo->first_take;
	second_take = philo->second_take;

//	params->held_by[first_take] = -1;
	update_prev_used_by(first_take, philo->idx, params);
//	params->prev_used_by[first_take] = (ssize_t)philo->idx;
	pthread_mutex_unlock(&params->fork_mutex[first_take]);

//	params->held_by[second_take] = -1;
//	params->prev_used_by[second_take] = (ssize_t)philo->idx;
	update_prev_used_by(second_take, philo->idx, params);
	pthread_mutex_unlock(&params->fork_mutex[second_take]);

	return (SUCCESS);
}

