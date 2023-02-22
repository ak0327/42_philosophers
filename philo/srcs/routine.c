/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/17 13:12:09 by takira            #+#    #+#             */
/*   Updated: 2023/02/22 10:20:23 by takira           ###   ########.fr       */
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
	t_params	*params;
	time_t		elapsed_time;
	bool		is_died;

	params = philo->params_ptr;
	pthread_mutex_lock(&params->died_mutex);
	is_died = params->is_died;
	pthread_mutex_unlock(&params->died_mutex);
	if (is_died)
		return (true);
	pthread_mutex_lock(&philo->philo_mutex);
	elapsed_time = get_unix_time_ms() - philo->start_time;
	pthread_mutex_unlock(&philo->philo_mutex);

	if (elapsed_time >= params->time_to_die)
	{
		pthread_mutex_lock(&params->died_mutex);
		params->is_died = true;
		params->died_idx = (ssize_t)philo->idx;
		print_msg(philo->idx, TYPE_DIED, params);
		params->died_idx = -1;
		pthread_mutex_unlock(&params->died_mutex);
		philo->is_continue = false;
		return (true);
	}
	return (false);
}

void	start_eating(t_philo_info *philo)
{
	t_params		*params;
	bool			is_died;

	params = philo->params_ptr;
	pthread_mutex_lock(&params->died_mutex);
	is_died = params->is_died;
	pthread_mutex_unlock(&params->died_mutex);
	if (is_died)
		return ;
//	params->state[philo->idx] = STATE_EATING;
	print_msg(philo->idx, TYPE_EATING, params);

	pthread_mutex_lock(&philo->philo_mutex);
	philo->start_time = get_unix_time_ms();
	pthread_mutex_unlock(&philo->philo_mutex);

	usleep(params->time_to_eat * 1000);
}

void	start_sleeping(t_philo_info *philo)
{
	t_params		*params;
	bool			is_died;

	params = philo->params_ptr;
	pthread_mutex_lock(&params->died_mutex);
	is_died = params->is_died;
	pthread_mutex_unlock(&params->died_mutex);
	if (is_died)
		return ;
//	params->state[philo->idx] = STATE_SLEEPING;
	print_msg(philo->idx, TYPE_SLEEPING, params);
	usleep(params->time_to_sleep * 1000);
}

void	start_thinking(t_philo_info *philo)
{
	t_params		*params;
	bool			is_died;

	params = philo->params_ptr;
	pthread_mutex_lock(&params->died_mutex);
	is_died = params->is_died;
	pthread_mutex_unlock(&params->died_mutex);

	if (is_died)
		return ;
//	params->state[philo->idx] = STATE_THINKING;
	print_msg(philo->idx, TYPE_THINKING, params);
}

void	update_eat_times(t_philo_info *philo)
{
	size_t	eat_times;

	pthread_mutex_lock(&philo->philo_mutex);
	philo->eat_times++;
	pthread_mutex_unlock(&philo->philo_mutex);

	eat_times = philo->eat_times;
	if (philo->params_ptr->must_eat_times >= 0 \
		&& (ssize_t)eat_times >= philo->params_ptr->must_eat_times)
		philo->is_continue = false;
}

bool	get_is_continue(t_philo_info *philo)
{
	bool	is_continue;

	pthread_mutex_lock(&philo->philo_mutex);
	is_continue = philo->is_continue;
	pthread_mutex_unlock(&philo->philo_mutex);
	return (is_continue);
}

void	*routine(void *v_philo_info)
{
	t_philo_info	*philo;

	philo = (t_philo_info *)v_philo_info;
	while (true)
	{
		take_forks(philo);
		start_eating(philo);
		put_forks(philo);
		update_eat_times(philo);
		start_sleeping(philo);
		start_thinking(philo);
		if (!get_is_continue(philo))
			break ;
	}
	return (NULL);
}

time_t	get_start_time(t_philo_info *philo)
{
	time_t	start_time;

	pthread_mutex_lock(&philo->philo_mutex);
	start_time = philo->start_time;
	pthread_mutex_unlock(&philo->philo_mutex);
	return (start_time);
}

int	is_philo_died(t_params *params)
{
	const time_t	now_time = get_unix_time_ms();
	bool			is_died;
	time_t			start_time;
	size_t			idx;

	idx = 0;
	pthread_mutex_lock(&params->died_mutex);
	is_died = params->is_died;
	if (is_died)
		return (true);
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

size_t	get_eat_times(t_philo_info *philo)
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

void	terminate_philo(t_params *params)
{
	size_t	idx;

	idx = 0;
	while (idx < params->num_of_philos)
	{
		pthread_mutex_lock(&params->philo_info[idx].philo_mutex);
		params->philo_info[idx].is_continue = false;
		pthread_mutex_unlock(&params->philo_info[idx].philo_mutex);
		idx++;
	}
}