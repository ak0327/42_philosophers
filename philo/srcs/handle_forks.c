/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_forks.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/20 09:56:44 by takira            #+#    #+#             */
/*   Updated: 2023/02/24 15:58:51 by takira           ###   ########.fr       */
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

static int	update_prev_used(size_t fork_idx, size_t used_by, t_params *params)
{
	if (pthread_mutex_lock(&params->prev_used_mutex[fork_idx]) != SUCCESS)
		return (PROCESS_ERROR);
	params->prev_used_by[fork_idx] = (ssize_t)used_by;
	if (pthread_mutex_unlock(&params->prev_used_mutex[fork_idx]) != SUCCESS)
		return (PROCESS_ERROR);
	return (SUCCESS);
}

static int	take_second_fork(t_philo_info *philo)
{
	t_params		*params;

	params = philo->params_ptr;
	if (philo->first_take == philo->second_take)
	{
		pthread_mutex_unlock(&params->fork_mutex[philo->first_take]);
		wait_while_philo_alive(params);
		return (PHILO_DIED);
	}
	if (pthread_mutex_lock(&params->fork_mutex[philo->second_take]) != SUCCESS)
		return (PROCESS_ERROR);
	if (print_msg(philo->idx, TYPE_FORK, params) == PHILO_DIED)
	{
		pthread_mutex_unlock(&params->fork_mutex[philo->first_take]);
		pthread_mutex_unlock(&params->fork_mutex[philo->second_take]);
		return (PHILO_DIED);
	}
	return (SUCCESS);
}

int	take_forks(t_philo_info *philo)
{
	t_params		*params;
	ssize_t			first_prev;
	ssize_t			second_prev;

	params = philo->params_ptr;
	first_prev = get_prev_used_by(philo->first_take, params);
	second_prev = get_prev_used_by(philo->second_take, params);
	if (is_some_philo_died(params) == PHILO_DIED)
		return (PHILO_DIED);
	if (first_prev == (ssize_t)philo->idx || second_prev == (ssize_t)philo->idx)
		return (CONTINUE);
	if (pthread_mutex_lock(&params->fork_mutex[philo->first_take]) != SUCCESS)
		return (PROCESS_ERROR);
	if (is_some_philo_died(params) == PHILO_DIED \
	|| print_msg(philo->idx, TYPE_FORK, params) == PHILO_DIED)
	{
		pthread_mutex_unlock(&params->fork_mutex[philo->first_take]);
		return (PHILO_DIED);
	}
	return (take_second_fork(philo));
}

int	put_forks(t_philo_info *philo, int prev_ret_val)
{
	t_params	*params;
	size_t		first_take;
	size_t		second_take;

	if (prev_ret_val == PROCESS_ERROR)
		return (prev_ret_val);
	params = philo->params_ptr;
	first_take = philo->first_take;
	second_take = philo->second_take;
	if (update_prev_used(first_take, philo->idx, params) != SUCCESS)
		return (PROCESS_ERROR);
	if (pthread_mutex_unlock(&params->fork_mutex[first_take]) != SUCCESS)
		return (PROCESS_ERROR);
	if (update_prev_used(second_take, philo->idx, params) != SUCCESS)
		return (PROCESS_ERROR);
	if (pthread_mutex_unlock(&params->fork_mutex[second_take]) != SUCCESS)
		return (PROCESS_ERROR);
	return (SUCCESS);
}
