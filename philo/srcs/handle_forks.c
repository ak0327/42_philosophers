/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_forks.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/20 09:56:44 by takira            #+#    #+#             */
/*   Updated: 2023/02/22 17:32:14 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	take_forks(t_philo_info *philo)
{
	t_params	*params;
	size_t		first_take;
	size_t		second_take;
	size_t		idx;

	params = philo->params_ptr;
	first_take = philo->first_take;
	second_take = philo->second_take;
	idx = philo->idx;

	if (params->prev_used_by[first_take] == (ssize_t)philo->idx \
	||	params->prev_used_by[second_take] == (ssize_t)philo->idx)
		return (FAILURE);

	pthread_mutex_lock(&params->fork_mutex[first_take]);
//	params->held_by[first_take] = (ssize_t)idx;
	print_msg(idx, TYPE_FORK, params);

	pthread_mutex_lock(&params->fork_mutex[second_take]);
//	params->held_by[second_take] = (ssize_t)philo->idx;
	print_msg(idx, TYPE_FORK, params);
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
	params->prev_used_by[first_take] = (ssize_t)philo->idx;
	pthread_mutex_unlock(&params->fork_mutex[first_take]);

//	params->held_by[second_take] = -1;
	params->prev_used_by[second_take] = (ssize_t)philo->idx;
	pthread_mutex_unlock(&params->fork_mutex[second_take]);

	return (SUCCESS);
}

