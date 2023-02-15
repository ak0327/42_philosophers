/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_philo.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/15 19:50:32 by takira            #+#    #+#             */
/*   Updated: 2023/02/15 21:24:55 by takira           ###   ########.fr       */
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

	philo->no = init_philo_no(args->num_of_philo);
	if (!philo->no)
		is_failure = true;
	philo->forks = init_forks(args->num_of_philo);
	if (!philo->forks)
		is_failure = true;
	if (pthread_mutex_init(&philo->waiter, NULL) != SUCCESS)
		is_failure = true;
	if (pthread_mutex_init(&philo->print_console, NULL) != SUCCESS)
		is_failure = true;

	if (!is_failure)
		return (philo);
	free(philo->no);
	free(philo->forks);
	free(philo);
	return (NULL);
}
