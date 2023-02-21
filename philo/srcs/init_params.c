/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_params.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/17 10:00:01 by takira            #+#    #+#             */
/*   Updated: 2023/02/20 14:18:33 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	init_params(int argc, char **argv, t_params **params)
{
	int	ret_value;

	if (!(5 <= argc && argc <= 6))
		return (INVALID_ARG_COUNT);
	*params = (t_params *)malloc(sizeof(t_params));
	if (!*params)
		return (PROCESS_ERROR);

	memset(*params, 0, sizeof(t_params));

	ret_value = get_input_args(argv, *params);
	if (ret_value !=SUCCESS)
	{
		free(*params);
		return (ret_value);
	}
	ret_value = init_thread(*params);
	if (ret_value != SUCCESS)
	{
		free(*params);
		return (ret_value);
	}
	(*params)->rev_philo_idx = (*params)->num_of_philos - 1;
	if ((*params)->num_of_philos % 2 == 0)
		(*params)->is_rev_exist = false;
	else
		(*params)->is_rev_exist = true;

	(*params)->is_died = false;
	(*params)->died_philo = -1;
	return (ret_value);
}

static pthread_t *init_philo_no(size_t num_of_philo)
{
	pthread_t	*philo_no;

	philo_no = (pthread_t *)malloc(sizeof(pthread_t) * (num_of_philo + 1));
	if (!philo_no)
		return (NULL);
	memset(philo_no, 0, sizeof(pthread_t) * num_of_philo);
	return (philo_no);
}

/* create mutex every lock_fork */
/*
static pthread_mutex_t	*init_forks(size_t num_of_philo)
{
	pthread_mutex_t	*fork_arr;
	size_t			idx;

	fork_arr = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t) * (num_of_philo + 1));
	if (!fork_arr)
		return (NULL);
	memset(fork_arr, 0, sizeof(pthread_mutex_t) * num_of_philo);
	idx = 0;
	while (idx < num_of_philo)
	{
		if (pthread_mutex_init(&fork_arr[idx], NULL) != SUCCESS)
			return (NULL);
		idx++;
	}
	return (fork_arr);
}
*/

static int	init_mutex(t_params **params)
{
	size_t	idx;

	if (pthread_mutex_init(&(*params)->lock_fork, NULL) != SUCCESS)
		return (FAILURE);

	if (pthread_mutex_init(&(*params)->lock_waiter, NULL) != SUCCESS)
		return (FAILURE);

	if (pthread_mutex_init(&(*params)->lock_died, NULL) != SUCCESS)
		return (FAILURE);

	if (pthread_mutex_init(&(*params)->lock_print, NULL) != SUCCESS)
		return (FAILURE);

	if (pthread_mutex_init(&(*params)->lock_state, NULL) != SUCCESS)
		return (FAILURE);

	(*params)->lock_each_fork = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t) * (*params)->num_of_philos);
	if (!(*params)->lock_each_fork)
		return (FAILURE);
	idx = 0;
	while (idx < (*params)->num_of_philos)
	{
		if (pthread_mutex_init(&(*params)->lock_each_fork[idx], NULL) != SUCCESS)
		{
			free((*params)->lock_each_fork);
			return (FAILURE);
		}
		idx++;
	}

	return (SUCCESS);
}

int	init_thread(t_params *params)
{
	bool	is_process_failure;

	params->tid = init_philo_no(params->num_of_philos);
	if (!params->tid)
		is_process_failure = true;

	params->fork_arr = (int *)malloc(sizeof(int) * params->num_of_philos);
	if (!params->fork_arr)
		is_process_failure = true;
	memset(params->fork_arr, 0, sizeof(int) * params->num_of_philos);

	params->eat_times = (size_t *)malloc(sizeof(ssize_t) * params->num_of_philos);
	if (!params->fork_arr)
		is_process_failure = true;
	memset(params->eat_times, 0, sizeof(size_t) * params->num_of_philos);

	params->state = (int *)malloc(sizeof(int) * params->num_of_philos);
	if (!params->state)
		is_process_failure = true;
	memset(params->state, STATE_THINKING, sizeof(int) * params->num_of_philos);

	if (init_mutex(&params) != SUCCESS)
		is_process_failure = true;

//	params->wait_queue = NULL;

	if (!is_process_failure)
		return (SUCCESS);
	free_allocs(params);
	return (FAILURE);
}
