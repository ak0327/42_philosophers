/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_params.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/17 10:00:01 by takira            #+#    #+#             */
/*   Updated: 2023/02/22 11:47:23 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	init_mutex(t_params *params);
static int	init_alloc(t_params **params);

int	init_params(int argc, char **argv, t_params **params)
{
	int	is_process_success;

	if (!(5 <= argc && argc <= 6))
		return (INVALID_ARG_COUNT);
	*params = (t_params *)malloc(sizeof(t_params));
	if (!*params)
		return (PROCESS_ERROR);

	memset(*params, 0, sizeof(t_params));

	is_process_success = true;
	is_process_success |= get_input_args(argv, *params);
	is_process_success |= init_alloc(params);
	is_process_success |= init_mutex(*params);

	(*params)->is_died = false;
	(*params)->died_idx = -1;

	if (is_process_success)
		return (SUCCESS);
	free_params(params);
	return (FAILURE);
}

static int	init_mutex(t_params *params)
{
	size_t	idx;

	params->fork_mutex = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t) * params->num_of_philos);
	if (!params->fork_mutex)
		return (FAILURE);

	idx = 0;
	while (idx < params->num_of_philos)
	{
		if (pthread_mutex_init(&params->fork_mutex[idx], NULL) != SUCCESS)
			return (FAILURE);
		if (pthread_mutex_init(&params->philo_info[idx].philo_mutex, NULL) != SUCCESS)
			return (FAILURE);

//		if (pthread_mutex_init(&params->philo_info[idx].eat_times_mutex, NULL) != SUCCESS)
//			return (FAILURE);
//		if (pthread_mutex_init(&params->philo_info[idx].start_time_mutex, NULL) != SUCCESS)
//			return (FAILURE);
		idx++;
	}

	if (pthread_mutex_init(&params->print_mutex, NULL) != SUCCESS)
		return (FAILURE);

	if (pthread_mutex_init(&params->died_mutex, NULL) != SUCCESS)
		return (FAILURE);

	return (SUCCESS);
}

static int	init_alloc(t_params **params)
{
	size_t	idx;
	size_t	swap_tmp;

	(*params)->philo_tid = (pthread_t *)malloc(sizeof(pthread_t) * (*params)->num_of_philos);
	(*params)->philo_info = (t_philo_info *)malloc(sizeof(t_philo_info) * (*params)->num_of_philos);

	(*params)->state = (int *)malloc(sizeof(int) * (*params)->num_of_philos);
	(*params)->held_by = (ssize_t *)malloc(sizeof(ssize_t) * (*params)->num_of_philos);
	(*params)->prev_used_by = (ssize_t *)malloc(sizeof(ssize_t) * (*params)->num_of_philos);

	if (!(*params)->philo_tid || !(*params)->philo_info \
 || !(*params)->state || !(*params)->held_by || !(*params)->prev_used_by)
		return (FAILURE);

	idx = 0;
	while (idx < (*params)->num_of_philos)
	{
		(*params)->philo_info[idx].idx = idx;

		(*params)->philo_info[idx].first_take = idx;
		(*params)->philo_info[idx].second_take = (idx + 1) % (*params)->num_of_philos;
		if (idx % 2 == 1)
		{
			swap_tmp = (*params)->philo_info[idx].first_take;
			(*params)->philo_info[idx].first_take = (*params)->philo_info[idx].second_take;
			(*params)->philo_info[idx].second_take = swap_tmp;
		}
//		(*params)->philo_info[idx].first_take = min_size(idx, (idx + 1) % (*params)->num_of_philos);
//		(*params)->philo_info[idx].second_take = max_size(idx, (idx + 1) % (*params)->num_of_philos);
		(*params)->philo_info[idx].start_time = 0;
		(*params)->philo_info[idx].eat_times = 0;
		(*params)->philo_info[idx].params_ptr = *params;
		(*params)->philo_info[idx].is_meet_eat_times = true;

		(*params)->state[idx] = FORK_DIRTY;
		(*params)->prev_used_by[idx] = -1;
		(*params)->held_by[idx] = -1;
		idx++;
	}
//	if ((*params)->num_of_philos % 2 == 1)
//	{
//		idx = (*params)->num_of_philos - 1;
//		swap_tmp = (*params)->philo_info[idx].first_take;
//		(*params)->philo_info[idx].first_take = (*params)->philo_info[idx].second_take;
//		(*params)->philo_info[idx].second_take = swap_tmp;
//	}
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

//	if (pthread_mutex_destroy(&params->died_mutex) != SUCCESS)
//		return (PROCESS_ERROR);

	free_params(&params);

	return (SUCCESS);
}
