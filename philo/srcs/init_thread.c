/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_philo.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/15 19:50:32 by takira            #+#    #+#             */
/*   Updated: 2023/02/17 11:12:39 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static pthread_t *init_philo_no(size_t num_of_philo)
{
	pthread_t	*philo_no;

	philo_no = (pthread_t *)malloc(sizeof(pthread_t) * num_of_philo);
	if (!philo_no)
		return (NULL);
	memset(philo_no, 0, sizeof(pthread_t) * num_of_philo);
	return (philo_no);
}

/* create mutex every fork */
static pthread_mutex_t	*init_forks(size_t num_of_philo)
{
	pthread_mutex_t	*forks;
	size_t			idx;

	forks = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t) * num_of_philo);
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

int	init_thread(t_params *params)
{
	bool	is_failure;

	// create pthread_t every philo
	params->philo_no = init_philo_no(params->num_of_philos);
	if (!params->philo_no)
		is_failure = true;

	// create mutex for fork
	params->forks = init_forks(params->num_of_philos);
	if (!params->forks)
		is_failure = true;

	// create mutex for waiter
	if (pthread_mutex_init(&params->waiter, NULL) != SUCCESS)
		is_failure = true;

	// create mutex for print
	if (pthread_mutex_init(&params->print_console, NULL) != SUCCESS)
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
