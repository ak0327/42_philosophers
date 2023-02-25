/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_params.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/17 10:00:01 by takira            #+#    #+#             */
/*   Updated: 2023/02/25 11:01:38 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

static void	init_alloc_elem(t_info **info, size_t idx)
{
	size_t	second_take;

	(*info)->philo_info[idx].idx = idx;
	(*info)->philo_info[idx].first_take = idx;
	second_take = (idx + 1) % (*info)->num_of_philos;
	(*info)->philo_info[idx].second_take = second_take;
	if (idx % 2 == 1)
	{
		(*info)->philo_info[idx].first_take = second_take;
		(*info)->philo_info[idx].second_take = idx;
	}
	(*info)->philo_info[idx].eat_times = 0;
	(*info)->philo_info[idx].info_ptr = *info;
	(*info)->philo_info[idx].is_meet_eat_times = false;
	(*info)->prev_used_by[idx] = -1;
}

static int	init_alloc(t_info **info)
{
	size_t	idx;

	(*info)->philo_tid = \
	(pthread_t *)malloc(sizeof(pthread_t) * (*info)->num_of_philos);
	(*info)->philo_info = \
	(t_philo_info *)malloc(sizeof(t_philo_info) * (*info)->num_of_philos);
	if (!(*info)->philo_tid || !(*info)->philo_info )
		return (FAILURE);
	idx = 0;
	while (idx < (*info)->num_of_philos)
	{
		init_alloc_elem(info, idx);
		idx++;
	}
	return (SUCCESS);
}

int	init_params(int argc, char **argv, t_info **info)
{
	int	is_process_success;

	if (!(5 <= argc && argc <= 6))
		return (INVALID_ARG_COUNT);
	*info = (t_info *)malloc(sizeof(t_info));
	if (!*info)
		return (PROCESS_ERROR);
	memset(*info, 0, sizeof(t_info));
	is_process_success = true;
	is_process_success |= get_input_args(argv, *info);
	is_process_success |= init_alloc(info);
	(*info)->is_died = PHILO_ALIVE;
	(*info)->died_idx = -1;
	(*info)->is_sim_fin = CONTINUE;
	if (is_process_success)
		return (SUCCESS);
	free_info(info);
	return (FAILURE);
}
