/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_params.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/17 10:00:01 by takira            #+#    #+#             */
/*   Updated: 2023/02/18 17:06:56 by takira           ###   ########.fr       */
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
	if ((*params)->num_of_philos % 2 == 0)
		(*params)->wait_philo_idx = -1;
	else
		(*params)->wait_philo_idx = (ssize_t)(*params)->num_of_philos - 1;
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
	pthread_mutex_t	*forks;
	size_t			idx;

	forks = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t) * (num_of_philo + 1));
	if (!forks)
		return (NULL);
	memset(forks, 0, sizeof(pthread_mutex_t) * num_of_philo);
	idx = 0;
	while (idx < num_of_philo)
	{
		if (pthread_mutex_init(&forks[idx], NULL) != SUCCESS)
			return (NULL);
		idx++;
	}
	return (forks);
}
*/

int	init_mutex(t_params **params)
{
	if (pthread_mutex_init(&(*params)->lock_fork, NULL) != SUCCESS)
		return (FAILURE);

	if (pthread_mutex_init(&(*params)->lock_waiter, NULL) != SUCCESS)
		return (FAILURE);

	if (pthread_mutex_init(&(*params)->lock_died, NULL) != SUCCESS)
		return (FAILURE);

	if (pthread_mutex_init(&(*params)->lock_print, NULL) != SUCCESS)
		return (FAILURE);

	if (pthread_mutex_init(&(*params)->lock_eat_times, NULL) != SUCCESS)
		return (FAILURE);

	return (SUCCESS);
}

int	init_thread(t_params *params)
{
	bool	is_process_failure;

	// create pthread_t every philo
	params->philo_no = init_philo_no(params->num_of_philos);
	if (!params->philo_no)
		is_process_failure = true;

	// create mutex for lock_fork
//	params->forks = init_forks(params->num_of_philos);

	params->forks = (int *)malloc(sizeof(int) * params->num_of_philos);
	if (!params->forks)
		is_process_failure = true;
	memset(params->forks, 0, sizeof(int) * params->num_of_philos);

	params->eat_times = (ssize_t *)malloc(sizeof(ssize_t) * params->num_of_philos);
	if (!params->forks)
		is_process_failure = true;
	memset(params->eat_times, 0, sizeof(ssize_t) * params->num_of_philos);

	if (init_mutex(&params) != SUCCESS)
		is_process_failure = true;

	params->wait_queue = NULL;

	if (!is_process_failure)
		return (SUCCESS);
	free(params->philo_no);
	free(params->forks);
	params->philo_no = NULL;
	params->forks = NULL;
	return (FAILURE);
}
