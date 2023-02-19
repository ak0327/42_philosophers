/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/17 13:12:09 by takira            #+#    #+#             */
/*   Updated: 2023/02/20 00:41:57 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	print_state_w_lock(t_params *params, size_t id)
{
	size_t	idx;
	char	*str;

	idx = 0;

	pthread_mutex_lock(&params->lock_print);

	printf("# state w (%zu) ::", id);
	pthread_mutex_lock(&params->lock_state);
	while (idx < params->num_of_philos)
	{
		if (params->state[idx] == STATE_THINKING)
			str = "thinking";
		if (params->state[idx] == STATE_HUNGRY)
			str = "HUNGRY";
		if (params->state[idx] == STATE_EATING)
			str = "eating";
		if (params->state[idx] == STATE_SLEEPING)
			str = "sleeping";
		if (params->state[idx] == STATE_WAITING)
			str = "waiting";
		printf("[%zu]%-10s", idx, str);
		idx++;
		if (idx < params->num_of_philos)
			printf(", ");
		else
			printf("\n");
	}
	pthread_mutex_unlock(&params->lock_state);

	pthread_mutex_unlock(&params->lock_print);
}

void	print_state_wo_lock(t_params *params, size_t id)
{
	size_t	idx;
	char	*str;

	idx = 0;
	pthread_mutex_lock(&params->lock_print);

	printf("# state wo(%zu) ::", id);
	while (idx < params->num_of_philos)
	{
		if (params->state[idx] == STATE_THINKING)
			str = "thinking";
		if (params->state[idx] == STATE_HUNGRY)
			str = "HUNGRY  ";
		if (params->state[idx] == STATE_EATING)
			str = "eating  ";
		if (params->state[idx] == STATE_SLEEPING)
			str = "sleeping";
		printf("[%zu]%s, ", idx, str);
		idx++;
	}
	printf("\n");
	pthread_mutex_unlock(&params->lock_print);

}

int	check_died(t_params *params, size_t idx)
{
	const time_t	now_time = get_unix_time_ms();
	const time_t	std_time = params->philo_info[idx].start_time;
	const time_t	time_to_die = params->time_to_die;

	return (now_time - std_time >= time_to_die);
}

bool	is_meet_mast_eat_times(t_params *params)
{
	const ssize_t	must_eat_times = params->must_eat_times;
	size_t			idx;

	if (must_eat_times < 0)
		return (false);
	idx = 0;
	while (idx < params->num_of_philos)
	{
		if (params->eat_times[idx] < params->must_eat_times)
			return (false);
		idx++;
	}
	return (true);
}

size_t	min_size(size_t a, size_t b)
{
	if (a <= b)
		return (a);
	return (b);
}

size_t	max_size(size_t a, size_t b)
{
	if (a >= b)
		return (a);
	return (b);
}

int	take_forks(t_params *params, t_philo_info *philo)
{
	const size_t	first_take = philo->first_take;;
	const size_t	second_take = philo->second_take;

	pthread_mutex_lock(&params->lock_state);
	params->state[philo->idx] = STATE_HUNGRY;

	if (params->state[philo->left_philo_idx] != STATE_EATING \
	&& params->state[philo->right_philo_idx] != STATE_EATING)
		params->state[philo->idx] = STATE_WAITING;

	pthread_mutex_unlock(&params->lock_state);

//	print_state_w_lock(params, philo->idx);

	while (params->state[philo->idx] == STATE_HUNGRY) //wait
		if (params->is_died)
			return (PHILO_DIED);

//	printf("\n");
//	printf("[#Take] %zu-1 will:%zu(%s)\n", philo->idx, first_take, controlled);
	pthread_mutex_lock(&params->lock_each_fork[first_take]);
//	printf("[#Take] %zu-2 take:%zu(%s)\n", philo->idx, first_take, controlled);
//	printf("[#Take] %zu-3 will:%zu(%s)\n", philo->idx, second_take, controlled);
	pthread_mutex_lock(&params->lock_each_fork[second_take]);
//	printf("[#Take] %zu-4 take:%zu(%s)\n", philo->idx, second_take, controlled);
//	printf("\n");

	pthread_mutex_lock(&params->lock_state);
	params->state[philo->idx] = STATE_EATING;
	pthread_mutex_unlock(&params->lock_state);

	print_state_w_lock(params, philo->idx);
	return (SUCCESS);
}

bool	is_philo_can_eating(t_params *params, size_t idx)
{
	const size_t	left_idx = params->philo_info[idx].left_philo_idx;
	const size_t	right_idx = params->philo_info[idx].right_philo_idx;

	if ((params->state[idx] == STATE_HUNGRY \
	&& params->state[left_idx] != STATE_EATING \
	&& params->state[right_idx] != STATE_EATING))
		return (true);

	if (params->state[idx] != STATE_EATING \
	&& params->state[left_idx] != STATE_EATING \
	&& params->state[right_idx] != STATE_EATING)
	{
		if (params->state[left_idx] == STATE_HUNGRY \
		|| params->state[right_idx] == STATE_HUNGRY)
			return (false);
		return (true);
	}
	return (false);
}

int	put_forks(t_params *params, t_philo_info *philo)
{
	print_state_w_lock(params, philo->idx);

	pthread_mutex_lock(&params->lock_state);

	params->state[philo->idx] = STATE_THINKING;

	if (is_philo_can_eating(params, philo->left_philo_idx))
	{
		params->state[philo->left_philo_idx] = STATE_WAITING;
	}
	if (is_philo_can_eating(params, philo->right_philo_idx))
	{
		params->state[philo->right_philo_idx] = STATE_WAITING;
	}
	pthread_mutex_unlock(&params->lock_each_fork[philo->first_take]);
	pthread_mutex_unlock(&params->lock_each_fork[philo->second_take]);


	pthread_mutex_unlock(&params->lock_state);

	print_state_w_lock(params, philo->idx);

//	printf("\n");
//	printf("  [#Put] %zu-1 put %zu\n", philo->idx, philo->first_take);
//	printf("  [#Put] %zu-2 put %zu\n", philo->idx, philo->second_take);
//	printf("\n");

	return (SUCCESS);
}



int	check_philo_alieve(t_params *params, size_t idx, time_t std_time)
{
	if (params->is_died && params->died_philo != (ssize_t)idx)
		return (PHILO_DIED);
	if (check_died(params, idx))
	{
		if (!params->is_died)
		{
			pthread_mutex_lock(&params->lock_died);
			params->is_died = true;
			params->died_philo = (ssize_t)idx;
			pthread_mutex_unlock(&params->lock_died);
		}

		pthread_mutex_lock(&params->lock_print);
		print_msg(idx, TYPE_DIED, std_time + params->time_to_die, params);
		pthread_mutex_unlock(&params->lock_print);
		printf("died waiting time:%ld\n", get_unix_time_ms() - std_time);

		print_state_wo_lock(params, idx);
		return (PHILO_DIED);
	}
	return (PHILO_ALIVE);
}

void	*do_routine(void *v_philo)
{
	t_philo_info	*philo;
	t_params		*params;

	philo = (t_philo_info *)v_philo;
	params = philo->params;

//	print_state_w_lock(params, philo->idx);
	while (is_meet_mast_eat_times(params) == false)
	{

		/* take a fork */

		take_forks(params, philo);
		if (check_philo_alieve(params, philo->idx, philo->start_time) == PHILO_DIED)
		{
			put_forks(params, philo);
			return (NULL);
		}

		/* eat */
		pthread_mutex_lock(&params->lock_state);
		params->state[philo->idx] = STATE_EATING;
		pthread_mutex_unlock(&params->lock_state);

		pthread_mutex_lock(&params->lock_print);
		philo->start_time = get_unix_time_ms();
		print_msg(philo->idx, TYPE_FORK, philo->start_time, params);
		print_msg(philo->idx, TYPE_FORK, philo->start_time, params);
		print_msg(philo->idx, TYPE_EATING, get_unix_time_ms(), params);
		pthread_mutex_unlock(&params->lock_print);

		usleep(params->time_to_eat * 1000);

		/* put fork_arr */
		put_forks(params, philo);

//		pthread_mutex_lock(&params->lock_eat_times);
		params->eat_times[philo->idx]++;
//		pthread_mutex_unlock(&params->lock_eat_times);

		/* sleep */
		if (check_philo_alieve(params, philo->idx, philo->start_time) == PHILO_DIED)
			return (NULL);

		pthread_mutex_lock(&params->lock_print);
		print_msg(philo->idx, TYPE_SLEEPING, get_unix_time_ms(), params);
		pthread_mutex_unlock(&params->lock_print);

		usleep(params->time_to_sleep * 1000);

		/* think */
		if (check_philo_alieve(params, philo->idx, philo->start_time) == PHILO_DIED)
			return (NULL);

		pthread_mutex_lock(&params->lock_print);
		print_msg(philo->idx, TYPE_THINKING, get_unix_time_ms(), params);
		pthread_mutex_unlock(&params->lock_print);

	}
	return (NULL);
}
