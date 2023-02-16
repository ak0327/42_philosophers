/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_philo.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/15 19:50:32 by takira            #+#    #+#             */
/*   Updated: 2023/02/16 16:49:40 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static pthread_t *init_philo_no(int num_of_philo)
{
	pthread_t	*philo_no;

	philo_no = (pthread_t *)malloc(sizeof(pthread_t) * num_of_philo);
	if (!philo_no)
		return (NULL);
	memset(philo_no, 0, sizeof(pthread_t) * num_of_philo);
	return (philo_no);
}

/* create mutex every fork */
static pthread_mutex_t	*init_forks(int num_of_philo)
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

t_philo	*init_philo(t_args *args)
{
	t_philo	*philo;
	bool	is_failure;

	if (!args)
		return (NULL);
	is_failure = false;
	philo = (t_philo *)malloc(sizeof(t_philo));
	if (!philo)
		return (NULL);

	// create pthread_t every philo
	philo->no = init_philo_no(args->num_of_philo);
	if (!philo->no)
		is_failure = true;

	// create mutex for fork
	philo->forks = init_forks(args->num_of_philo);
	if (!philo->forks)
		is_failure = true;

	// create mutex for waiter
	if (pthread_mutex_init(&philo->waiter, NULL) != SUCCESS)
		is_failure = true;

	// create mutex for print
	if (pthread_mutex_init(&philo->print_console, NULL) != SUCCESS)
		is_failure = true;

	// if process failure
	if (!is_failure)
		return (philo);
	free(philo->no);
	free(philo->forks);
	free(philo);
	return (NULL);
}
