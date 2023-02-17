/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_philo.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/15 19:50:32 by takira            #+#    #+#             */
/*   Updated: 2023/02/17 21:28:41 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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

int	init_thread(t_params *params)
{
	bool	is_failure;

	// create pthread_t every philo
	params->philo_no = init_philo_no(params->num_of_philos);
	if (!params->philo_no)
		is_failure = true;

	// create mutex for lock_fork
//	params->forks = init_forks(params->num_of_philos);

	params->forks = (int *)malloc(sizeof(int) * params->num_of_philos);
	if (!params->forks)
		is_failure = true;
	memset(params->forks, 0, sizeof(int) * params->num_of_philos);

	params->eat_times = (ssize_t *)malloc(sizeof(ssize_t) * params->num_of_philos);
	if (!params->forks)
		is_failure = true;
	memset(params->eat_times, 0, sizeof(ssize_t) * params->num_of_philos);

	if (pthread_mutex_init(&params->lock_fork, NULL) != SUCCESS)
		is_failure = true;

	// create mutex for lock_waiter
	if (pthread_mutex_init(&params->lock_waiter, NULL) != SUCCESS)
		is_failure = true;

	// create mutex for print
	if (pthread_mutex_init(&params->lock_print, NULL) != SUCCESS)
		is_failure = true;

	// if process failure
	if (!is_failure)
		return (SUCCESS);
	free(params->philo_no);
	free(params->forks);
	params->philo_no = NULL;
	params->forks = NULL;
	return (FAILURE);
}
