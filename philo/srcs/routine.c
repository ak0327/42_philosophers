/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/17 13:12:09 by takira            #+#    #+#             */
/*   Updated: 2023/02/21 23:01:08 by takira           ###   ########.fr       */
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

bool	is_died(t_philo_info *philo)
{
	const time_t	now_time = get_unix_time_ms();
	const size_t	idx = philo->idx;
	t_params		*params;

	params = philo->params_ptr;
	if (params->is_died)
		return (true);
	if (now_time - philo->start_time >= params->time_to_die)
	{
		pthread_mutex_lock(&params->died_mutex);
		params->is_died = true;
		params->died_idx = (ssize_t)idx;
		print_msg(idx, TYPE_DIED, params);
		params->died_idx = -1;
		pthread_mutex_unlock(&params->died_mutex);
		philo->is_continue = false;
		return (true);
	}
	return (false);
}

void	start_eating(t_philo_info *philo)
{
	const time_t	now_time = get_unix_time_ms();
	t_params		*params;

	params = philo->params_ptr;
	if (params->is_died)
		return ;
	params->state[philo->idx] = STATE_EATING;
	print_msg(philo->idx, TYPE_EATING, params);
	philo->start_time = now_time;
	usleep(params->time_to_eat * 1000);
}

void	start_sleeping(t_philo_info *philo)
{
	t_params		*params;

	params = philo->params_ptr;
	if (params->is_died)
		return ;
	params->state[philo->idx] = STATE_SLEEPING;
	print_msg(philo->idx, TYPE_SLEEPING, params);
	usleep(params->time_to_sleep * 1000);
}

void	start_thinking(t_philo_info *philo)
{
	t_params		*params;

	params = philo->params_ptr;
	if (params->is_died)
		return ;
	params->state[philo->idx] = STATE_THINKING;
	print_msg(philo->idx, TYPE_THINKING, params);
}

void	*routine(void *v_philo_info)
{
	t_philo_info	*philo;

	philo = (t_philo_info *)v_philo_info;
	while (!philo->is_continue)
	{
		take_forks(philo);
		start_eating(philo);
		put_forks(philo);
		start_sleeping(philo);
		start_thinking(philo);
	}
	return (NULL);
}

void	check_philo_died(t_params *params)
{
	const time_t	now_time = get_unix_time_ms();
	time_t			start_time;
	size_t			idx;

	idx = 0;
	start_time = params->philo_info[idx].start_time;
	while (idx < params->num_of_philos)
	{
		if (now_time - start_time >= params->time_to_die)
		{
			pthread_mutex_lock(&params->died_mutex);
			params->is_died = true;
			params->died_idx = (ssize_t)idx;
			print_msg(idx, TYPE_DIED, params);
			params->died_idx = -1;
			pthread_mutex_unlock(&params->died_mutex);

			/*
			//// debug ////
			pthread_mutex_lock(&params->print_mutex);

			printf("\x1b[31mmonitor\x1b[0m \x1b[48;5;%03zum%zu\x1b[0m \x1b[31mis died\x1b[0m @ %zu\n", idx % 255, idx, now_time);
			pthread_mutex_unlock(&params->print_mutex);
			params->is_continue_monitor = false;

			///////////////
			 */
			return ;
		}
		idx++;
	}
}

bool	is_meet_must_eat_times(t_params *params)
{
	size_t	idx;
	size_t	must_eat_time;

	if (params->must_eat_times < 0)
		return (false);
	must_eat_time = (size_t)params->must_eat_times;
	idx = 0;
	while (idx < params->num_of_philos)
	{
		if (params->philo_info[idx].eat_times < must_eat_time)
			return (false);
		idx++;
	}
	return (true);
}

void	terminate_philo(t_params *params)
{
	size_t	idx;

	idx = 0;
	while (idx < params->num_of_philos)
	{
		params->philo_info[idx].is_continue = false;
		idx++;
	}
}

