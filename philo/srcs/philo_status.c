/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_status_check.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/22 11:22:42 by takira            #+#    #+#             */
/*   Updated: 2023/02/23 11:16:22 by takira           ###   ########.fr       */
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

bool	get_is_meet_eat_times(t_philo_info *philo)
{
	bool	is_meet_eat_times;

	pthread_mutex_lock(&philo->philo_mutex);
	is_meet_eat_times = philo->is_meet_eat_times;
	pthread_mutex_unlock(&philo->philo_mutex);
	return (is_meet_eat_times);
}

int	is_meet_must_eat_times(t_params *params, int prev_ret_val)
{
	size_t	idx;

	if (prev_ret_val != SUCCESS)
		return (prev_ret_val);
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

int	is_each_philo_meet_eat_times(t_philo_info *philo, int prev_ret_val)
{
	size_t		arg_must_eat_times;
	ssize_t		philo_eat_times;
	t_params	*params;

	if (prev_ret_val != SUCCESS)
		return (prev_ret_val);
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
