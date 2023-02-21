/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/17 13:12:09 by takira            #+#    #+#             */
/*   Updated: 2023/02/21 19:53:01 by takira           ###   ########.fr       */
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
	const time_t	now = get_unix_time_ms();
	const size_t	idx = philo->idx;
	t_params		*params;

	params = philo->params_ptr;
	if (params->is_died)
		return (true);
	if (now - philo->start_time >= params->time_to_die)
	{
		pthread_mutex_lock(&params->died_mutex);
		params->is_died = true;
		params->died_idx = (ssize_t)idx;
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
	print_msg(philo->idx, TYPE_EATING, now_time, params);
	philo->start_time = now_time;
	usleep(params->time_to_eat * 1000);
}

void	start_sleeping(t_philo_info *philo)
{
	const time_t	now_time = get_unix_time_ms();
	t_params		*params;

	params = philo->params_ptr;
	if (params->is_died)
		return ;
	params->state[philo->idx] = STATE_SLEEPING;
	print_msg(philo->idx, TYPE_SLEEPING, now_time, params);
	usleep(params->time_to_sleep * 1000);
}

void	start_thinking(t_philo_info *philo)
{
	const time_t	now_time = get_unix_time_ms();
	t_params		*params;

	params = philo->params_ptr;
	if (params->is_died)
		return ;
	params->state[philo->idx] = STATE_THINKING;
	print_msg(philo->idx, TYPE_THINKING, now_time, params);
}

void	*routine(void *v_philo_info)
{
	t_philo_info	*philo;
	t_params		*params;

	philo = (t_philo_info *)v_philo_info;
	params = philo->params_ptr;

//	printf("idx:%zu, fork:%zu->%zu\n", philo->idx, philo->first_take, philo->second_take);
	while (!philo->is_continue)
	{
//		printf("idx:%zu, start:%zu\n", philo->idx, philo->start_time);
		take_forks(philo);
		start_eating(philo);
		put_forks(philo);
		start_sleeping(philo);
		start_thinking(philo);
		if (is_died(philo))
			break ;
	}
	params->state[philo->idx] = STATE_TERMINATED;
	if (params->died_idx == (ssize_t)philo->idx)
		print_msg(philo->idx, TYPE_DIED, philo->start_time + params->time_to_die, params);
	return (NULL);
}

void	monitor(t_params *params)
{
	time_t		now_time;
	time_t		start_time;
	size_t		idx;

	now_time = get_unix_time_ms();
	idx = 0;
	while (idx < params->num_of_philos)
	{
		start_time = params->philo_info[idx].start_time;
		if (now_time - start_time >= params->time_to_die)
		{
			pthread_mutex_lock(&params->died_mutex);
			params->is_died = true;
			params->died_idx = (ssize_t)idx;
			pthread_mutex_unlock(&params->died_mutex);

			//// debug ////
			pthread_mutex_lock(&params->print_mutex);

			printf("\x1b[31mmonitor\x1b[0m \x1b[48;5;%03zum%zu\x1b[0m \x1b[31mis died\x1b[0m @ %zu\n", idx % 255, idx, now_time);
			pthread_mutex_unlock(&params->print_mutex);
			params->is_continue_monitor = false;
			///////////////
			break ;
		}
		idx++;
	}
}
