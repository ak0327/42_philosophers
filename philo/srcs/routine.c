/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/17 13:12:09 by takira            #+#    #+#             */
/*   Updated: 2023/02/21 18:48:34 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	print_eat_times(t_params *params)
{
	size_t idx;

	idx = 0;
	printf("start_eating times:");
	while (idx < params->num_of_philos)
	{
		printf("%zu", params->philo_info[idx].eat_times);
		idx++;
		if (idx < params->num_of_philos)
			printf(", ");
		else
			printf("\n");
	}
}

char	*get_state_str(int state)
{
	if (state == STATE_THINKING)
		return("start_thinking");
	if (state == STATE_HUNGRY)
		return("\x1b[33mHUNGRY\x1b[0m  ");
	if (state == STATE_WAITING)
		return("\x1b[33mWAITING\x1b[0m  ");
	if (state == STATE_EATING)
		return("start_eating  ");
	if (state == STATE_SLEEPING)
		return("start_sleeping");
	return("\x1b[31mDIED\x1b[0m    ");
}

bool	is_died(t_params *params)
{
	const time_t	now = get_unix_time_ms();
	t_philo_info	philo_info;
	size_t			idx;

	if (params->is_died)
		return (true);
	idx = 0;
	while (idx < params->num_of_philos)
	{
		philo_info = params->philo_info[idx];
		if (now - philo_info.start_time >= params->time_to_die)
		{
			params->is_died = true;
			params->died_idx = (ssize_t)idx;
			return (true);
		}
		idx++;
	}
	return (false);
}

void	start_eating(t_philo_info *philo)
{
	t_params		*params;

	params = philo->params_ptr;
	params->state[philo->idx] = STATE_EATING;
	philo->start_time = get_unix_time_ms();
	print_msg(philo->idx, TYPE_EATING, philo->start_time, params);
	usleep(params->time_to_eat * 1000);
}

void	start_sleeping(t_philo_info *philo)
{
	t_params		*params;

	params = philo->params_ptr;
	params->state[philo->idx] = STATE_SLEEPING;
	print_msg(philo->idx, TYPE_SLEEPING, philo->start_time, params);
	usleep(params->time_to_sleep * 1000);
}

void	start_thinking(t_philo_info *philo)
{
	t_params		*params;

	params = philo->params_ptr;
	params->state[philo->idx] = STATE_THINKING;
	print_msg(philo->idx, TYPE_THINKING, philo->start_time, params);
}

void	*routine(void *v_philo_info)
{
	t_philo_info	*philo;
	t_params		*params;

	philo = (t_philo_info *)v_philo_info;
	params = philo->params_ptr;

	while (!params->is_died)
	{
		take_forks(philo);
		start_eating(philo);
		put_forks(philo);
		if (is_died(params))
			break ;
		start_sleeping(philo);
		if (is_died(params))
			break ;
		start_thinking(philo);

		if (is_died(params))
			break ;
	}
	params->state[philo->idx] = STATE_TERMINATED;
	if (params->died_idx == (ssize_t)philo->idx)
		print_msg(philo->idx, TYPE_DIED, philo->start_time + params->time_to_die, params);
	return (NULL);
}

void	*monitor(void *v_params)
{
	const time_t	time_to_die = ((t_params *)v_params)->time_to_die;
	time_t			now_time;
	t_params		*params;
	time_t			start_time;
	size_t			idx;

	params = (t_params *)v_params;
	while (!params->is_died)
	{
		now_time = get_unix_time_ms();
		idx = 0;
		while (idx < params->num_of_philos && !params->is_died)
		{
			start_time = params->philo_info[idx].start_time;
			if (now_time - start_time >= time_to_die)
			{
				pthread_mutex_lock(&params->died_mutex);
				params->is_died = true;
				params->died_idx = (ssize_t)idx;
				pthread_mutex_unlock(&params->died_mutex);

				//// debug ////
				pthread_mutex_lock(&params->print_mutex);
				printf("\x1b[31mmonitor\x1b[0m \x1b[48;5;%03zum%zu\x1b[0m \x1b[31mis died\x1b[0m @", idx % 255, idx);
				print_timestamp();
				printf("\n");
				pthread_mutex_unlock(&params->print_mutex);
				///////////////
				break ;
			}
			idx++;
		}
	}
	return (NULL);
}
