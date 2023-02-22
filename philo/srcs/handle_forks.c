/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_forks.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/20 09:56:44 by takira            #+#    #+#             */
/*   Updated: 2023/02/22 12:11:52 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	take_forks(t_philo_info *philo)
{

//	if (params->prev_used_by[first_take] == (ssize_t)philo->idx)
//	{
//		params->held_by[first_take] = -1;
//		params->prev_used_by[first_take] = -1;
//	}
	pthread_mutex_lock(&philo->params_ptr->fork_mutex[philo->first_take]);
//	params->held_by[first_take] = (ssize_t)philo->idx;
	print_msg(philo->idx, TYPE_FORK, philo->params_ptr);


	pthread_mutex_lock(&philo->params_ptr->fork_mutex[philo->second_take]);
	//	params->held_by[second_take] = (ssize_t)philo->idx;
	print_msg(philo->idx, TYPE_FORK, philo->params_ptr);

//	if (params->held_by[first_take] != (ssize_t)philo->idx)
//	{
//		pthread_mutex_lock(&params->fork_mutex[first_take]);
//		params->held_by[first_take] = (ssize_t)philo->idx;
//	}
	return (SUCCESS);
}

int	put_forks(t_philo_info *philo)
{
//	params->held_by[first_take] = -1;
//	params->prev_used_by[first_take] = (ssize_t)philo->idx;
	pthread_mutex_unlock(&philo->params_ptr->fork_mutex[philo->first_take]);
//	params->held_by[second_take] = -1;
//	params->prev_used_by[second_take] = (ssize_t)philo->idx;
	pthread_mutex_unlock(&philo->params_ptr->fork_mutex[philo->second_take]);

	return (SUCCESS);
}

