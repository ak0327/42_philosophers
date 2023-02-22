/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_status_check.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/22 11:22:42 by takira            #+#    #+#             */
/*   Updated: 2023/02/22 12:15:14 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_status.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/22 11:22:42 by takira            #+#    #+#             */
/*   Updated: 2023/02/22 11:29:17 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"


static size_t	get_eat_times(t_philo_info *philo)
{
	size_t	eat_times;

	pthread_mutex_lock(&philo->philo_mutex);
	eat_times = philo->eat_times;
	pthread_mutex_unlock(&philo->philo_mutex);
	return (eat_times);
}

bool	is_meet_must_eat_times(t_params *params)
{
	size_t	idx;
	size_t	arg_must_eat_times;
	size_t	philo_eat_times;

	if (params->must_eat_times < 0)
		return (false);
	arg_must_eat_times = (size_t)params->must_eat_times;
	idx = 0;
	while (idx < params->num_of_philos)
	{
		philo_eat_times = get_eat_times(&params->philo_info[idx]);
//		printf("(%zu)%zu, ", idx, params->philo_info[idx].eat_times);
		if (philo_eat_times < arg_must_eat_times)
			return (false);
		idx++;
	}
	return (true);
}

bool	get_is_died(t_params *params, ssize_t *idx)
{
	bool	is_died;

	pthread_mutex_lock(&params->died_mutex);
	is_died = params->is_died;
	if (idx)
		*idx = params->died_idx;
	pthread_mutex_unlock(&params->died_mutex);
	return (is_died);
}

static time_t	get_start_time(t_philo_info *philo)
{
	time_t	start_time;

	pthread_mutex_lock(&philo->philo_mutex);
	start_time = philo->start_time;
	pthread_mutex_unlock(&philo->philo_mutex);
	return (start_time);
}

int	is_some_philo_died(t_params *params)
{
	const time_t	now_time = get_unix_time_ms();
	bool			is_died;
	time_t			start_time;
	size_t			idx;

	idx = 0;
	is_died = get_is_died(params, NULL);
	if (is_died)
		return (true);
	pthread_mutex_lock(&params->died_mutex);
	is_died = false;
	while (idx < params->num_of_philos)
	{
		start_time = get_start_time(&params->philo_info[idx]);
		if (now_time - start_time >= params->time_to_die)
		{
			params->is_died = true;
			params->died_idx = (ssize_t)idx;
			print_msg(idx, TYPE_DIED, params);
			params->died_idx = -1;

			is_died = true;
			printf("died dt:%ld\n", get_unix_time_ms() - start_time);

			/*
			//// debug ////
			pthread_mutex_lock(&params->print_mutex);

			printf("\x1b[31mmonitor\x1b[0m \x1b[48;5;%03zum%zu\x1b[0m \x1b[31mis died\x1b[0m @ %zu\n", idx % 255, idx, now_time);
			pthread_mutex_unlock(&params->print_mutex);
			params->is_continue_monitor = false;

			///////////////
			 */
			break ;
		}
		idx++;
	}
	pthread_mutex_unlock(&params->died_mutex);
	return (is_died);
}
