/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_status_check.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/22 11:22:42 by takira            #+#    #+#             */
/*   Updated: 2023/02/22 22:49:38 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "philo.h"

static ssize_t	get_eat_times(t_philo_info *philo)
{
	size_t	eat_times;

	if (pthread_mutex_lock(&philo->philo_mutex) != SUCCESS)
		return (-1);
	eat_times = philo->eat_times;
	if (pthread_mutex_unlock(&philo->philo_mutex) != SUCCESS)
		return (-1);
	return ((ssize_t)eat_times);
}

static bool	get_is_meet_eat_times(t_philo_info *philo)
{
	bool	is_meet_eat_times;

	pthread_mutex_lock(&philo->philo_mutex);
	is_meet_eat_times = philo->is_meet_eat_times;
	pthread_mutex_unlock(&philo->philo_mutex);
	return (is_meet_eat_times);
}

int	is_meet_must_eat_times(t_params *params, int prev_ret_value)
{
	size_t	idx;

	if (prev_ret_value != SUCCESS)
		return (prev_ret_value);
	if (params->must_eat_times < 0)
		return (CONTINUE);
	idx = 0;
	while (idx < params->num_of_philos)
	{
		if (!get_is_meet_eat_times(&params->philo_info[idx]))
			return (CONTINUE);
		idx++;
	}
	return (BREAK);
}

bool	get_is_meet_must_eat_times(t_philo_info *philo)
{
	bool	must_eat_times;

	pthread_mutex_lock(&philo->philo_mutex);
	must_eat_times = philo->is_meet_eat_times;
	pthread_mutex_unlock(&philo->philo_mutex);
	return (must_eat_times);
}

int	is_each_philo_meet_eat_times(t_philo_info *philo, int prev_ret_value)
{
	size_t		arg_must_eat_times;
	ssize_t		philo_eat_times;
	t_params	*params;

	if (prev_ret_value != SUCCESS)
		return (prev_ret_value);
	params = philo->params_ptr;
	if (params->must_eat_times < 0)
		return (CONTINUE);
	arg_must_eat_times = (size_t)params->must_eat_times;
	philo_eat_times = get_eat_times(philo);
	if (philo_eat_times == -1)
		return (PROCESS_ERROR);
	if ((size_t)philo_eat_times < arg_must_eat_times)
		return (CONTINUE);
	pthread_mutex_lock(&philo->philo_mutex);
	philo->is_meet_eat_times = true;
	pthread_mutex_unlock(&philo->philo_mutex);
	return (BREAK);
}

int	get_is_died(t_params *params, ssize_t *idx, int prev_ret_value)
{
	int	is_died;

	if (!(prev_ret_value == SUCCESS || prev_ret_value == CONTINUE))
		return (prev_ret_value);
	if (pthread_mutex_lock(&params->died_mutex) != SUCCESS)
		return (PROCESS_ERROR);
	is_died = params->is_died;
	if (idx)
		*idx = params->died_idx;
	if (pthread_mutex_unlock(&params->died_mutex) != SUCCESS)
		return (PROCESS_ERROR);
	return (is_died);
}

static struct timeval	get_start_time(t_philo_info *philo)
{
	struct timeval	start_time;

	pthread_mutex_lock(&philo->philo_mutex);
	start_time = philo->start_time;
	pthread_mutex_unlock(&philo->philo_mutex);
	return (start_time);
}

int	is_some_philo_died(t_params *params)
{
	struct timeval	now_tv;
	struct timeval	start_tv;
	time_t			delta_time;
	size_t			idx;
	int				is_died;

	idx = 0;
//	if (get_is_died(params, NULL, SUCCESS) == PHILO_DIED)
//		return (PHILO_DIED);
//	if (pthread_mutex_lock(&params->died_mutex) != SUCCESS)
//		return (PROCESS_ERROR);
	pthread_mutex_lock(&params->died_mutex);
	is_died = params->is_died;
	pthread_mutex_unlock(&params->died_mutex);

	if (is_died == PHILO_DIED)
		return (PHILO_DIED);

	gettimeofday(&now_tv, NULL);
	while (idx < params->num_of_philos)
	{
		start_tv = get_start_time(&params->philo_info[idx]);
		delta_time = (now_tv.tv_sec - start_tv.tv_sec) * 1000 \
		+ (now_tv.tv_usec - start_tv.tv_usec) / 1000;
		if (!get_is_meet_eat_times(&params->philo_info[idx]) \
		&& delta_time >= params->time_to_die)
		{
			pthread_mutex_lock(&params->died_mutex);
			params->is_died = PHILO_DIED;
			params->died_idx = (ssize_t)idx;
			pthread_mutex_unlock(&params->died_mutex);

			print_msg(idx, TYPE_DIED, params);
			printf(" died(%zu)\n", idx + 1);

			pthread_mutex_lock(&params->died_mutex);
			params->died_idx = -1;
			pthread_mutex_unlock(&params->died_mutex);
			return (PHILO_DIED);
		}
		idx++;
	}
	if (pthread_mutex_unlock(&params->died_mutex) != SUCCESS)
		return (PROCESS_ERROR);
	return (PHILO_ALIVE);
}
