/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/17 13:12:09 by takira            #+#    #+#             */
/*   Updated: 2023/02/18 18:44:13 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

//          can not eat
//          v  v
// waiting [0, 1, 2, 3, ...]
//                ^ can eat

// waiting		[3, ...]
// take forks	[2]
// popped 		[0, 1]
// new waiting	[0, 1, 3, ...]

int	check_died(t_params *params, size_t idx)
{
	const time_t	now_time = get_unix_time_ms();
	const time_t	std_time = params->philo_info[idx].std_time;
	const time_t	time_to_die = params->time_to_die;

	return (now_time - std_time >= time_to_die);
}

bool	is_meet_mast_eat_time(t_params *params)
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

int	take_forks(t_params *params, size_t idx)
{
	const size_t	num_of_philos = params->num_of_philos;
	size_t			left_idx;
	size_t			right_idx;
	int				ret_value;

	left_idx = idx % num_of_philos;
	right_idx = (idx + 1) % num_of_philos;

	ret_value = FAILURE;
//	pthread_mutex_lock(&params->lock_fork);
	if (params->forks[left_idx] == 0 && params->forks[right_idx] == 0)
	{
		params->forks[left_idx] = 1;
		params->forks[right_idx] = 1;
		ret_value = SUCCESS;
	}
//	pthread_mutex_unlock(&params->lock_fork);
	return (ret_value);
}

int	release_forks(t_params *params, size_t idx)
{
	const size_t	num_of_philos = params->num_of_philos;
	size_t			first_take;
	size_t			second_take;

	first_take = idx % num_of_philos;
	second_take = (idx + 1) % num_of_philos;
	if (idx % 2 == 1)
	{
		first_take = (idx + 1) % num_of_philos;
		second_take = idx % num_of_philos;
	}
	pthread_mutex_lock(&params->lock_fork);
	params->forks[first_take] = 0;
	params->forks[second_take] = 0;
	pthread_mutex_unlock(&params->lock_fork);
	return (SUCCESS);
}

int	check_philo_alieve(t_params *params, size_t idx, time_t std_time)
{
	if (check_died(params, idx))
	{
		pthread_mutex_lock(&params->lock_died);
		params->is_died = true;
		params->died_philo = (ssize_t)idx;
		pthread_mutex_unlock(&params->lock_died);

		pthread_mutex_lock(&params->lock_print);
		print_msg(idx, TYPE_DIED, std_time + params->time_to_die, params);
		pthread_mutex_unlock(&params->lock_print);
		return (PHILO_DIED);
	}
	if (params->is_died)
		return (PHILO_DIED);
	return (PHILO_ALIVE);
}

void	*do_routine(void *v_philo)
{
	t_each_philo	*philo;
	t_params		*params;

	philo = (t_each_philo *)v_philo;
	params = philo->params;
	while (is_meet_mast_eat_time(params) == false)
	{
		// waiting
		params->philo_info[philo->idx].is_allowed = false;
		usleep(10);
		pthread_mutex_lock(&params->lock_waiter);
		add_right(philo->wait, &params->wait_queue);
		pthread_mutex_unlock(&params->lock_waiter);

		while (params->philo_info[philo->idx].is_allowed == false)
		{
			if (check_philo_alieve(params, philo->idx, philo->std_time) == PHILO_DIED)
				return (NULL);
		}
		if (check_philo_alieve(params, philo->idx, philo->std_time) == PHILO_DIED)
			return (NULL);

		// eat
		pthread_mutex_lock(&params->lock_print);
		philo->std_time = get_unix_time_ms();
		print_msg(philo->idx, TYPE_FORK, philo->std_time, params);
		print_msg(philo->idx, TYPE_FORK, philo->std_time, params);
		print_msg(philo->idx, TYPE_EATING, get_unix_time_ms(), params);
		pthread_mutex_unlock(&params->lock_print);

		usleep(params->time_to_eat * 1000);

		// release forks
		release_forks(params, philo->idx);

//		pthread_mutex_lock(&params->lock_eat_times);
		params->eat_times[philo->idx]++;
//		pthread_mutex_unlock(&params->lock_eat_times);

		// sleep
		if (check_philo_alieve(params, philo->idx, philo->std_time) == PHILO_DIED)
			return (NULL);

		pthread_mutex_lock(&params->lock_print);
		print_msg(philo->idx, TYPE_SLEEPING, get_unix_time_ms(), params);
		pthread_mutex_unlock(&params->lock_print);

		usleep(params->time_to_sleep * 1000);

		// think
		if (check_philo_alieve(params, philo->idx, philo->std_time) == PHILO_DIED)
			return (NULL);

		pthread_mutex_lock(&params->lock_print);
		print_msg(philo->idx, TYPE_THINKING, get_unix_time_ms(), params);
		pthread_mutex_unlock(&params->lock_print);
	}
	return (NULL);
}

/*
// idx=0,	 -> 0, 1
// idx=1,	 -> 1, 2
// idx=len-1 -> len-1, 0

//int	take_forks(t_params *params, size_t idx)
//{
//	const size_t	num_of_philos = params->num_of_philos;
//	size_t			first_take;
//	size_t			second_take;
//
//	first_take = idx % num_of_philos;
//	second_take = (idx + 1) % num_of_philos;
//	if (idx % 2 == 1)
//	{
//		first_take = (idx + 1) % num_of_philos;
//		second_take = idx % num_of_philos;
//	}
//	if (params->wait_philo_idx != -1 && (size_t)params->wait_philo_idx == idx)
//	{
//		params->wait_philo_idx++;
//		params->wait_philo_idx %= (ssize_t)params->num_of_philos - 1;
//		return (FAILURE);
//	}
//	if (params->forks[first_take] == 0 && params->forks[second_take] == 0)
//	{
//		pthread_mutex_lock(&params->lock_fork);
//		params->forks[first_take] = 1;
//		params->forks[second_take] = 1;
//		pthread_mutex_unlock(&params->lock_fork);
//		return (SUCCESS);
//	}
//	return (FAILURE);
////	printf(" #DEGUB[%zu]take    %zu, %zu\n", idx, first_take, second_take);
//}

int	take_forks(t_params *params, size_t idx)
{
	const size_t	num_of_philos = params->num_of_philos;
	size_t			first_take;
	size_t			second_take;

	first_take = idx % num_of_philos;
	second_take = (idx + 1) % num_of_philos;
	if (idx + 1 == num_of_philos)
	{
		first_take = (idx + 1) % num_of_philos;
		second_take = idx % num_of_philos;
	}
	if (params->wait_philo_idx != -1 && (size_t)params->wait_philo_idx == idx)
	{
		params->wait_philo_idx++;
		params->wait_philo_idx %= (ssize_t)params->num_of_philos - 1;
		return (FAILURE);
	}
	if (params->forks[first_take] == 0 && params->forks[second_take] == 0)
	{
		pthread_mutex_lock(&params->lock_fork);
		params->forks[first_take] = 1;
		params->forks[second_take] = 1;
		pthread_mutex_unlock(&params->lock_fork);
		return (SUCCESS);
	}
	return (FAILURE);
//	printf(" #DEGUB[%zu]take    %zu, %zu\n", idx, first_take, second_take);
}

int	take_left_forks(t_params *params, size_t idx)
{
	const size_t	num_of_philos = params->num_of_philos;
	size_t			first_take;

	first_take = idx % num_of_philos;
	if (idx % 2 == 1)
		first_take = (idx + 1) % num_of_philos;
	if (params->forks[first_take] == 0)
	{
		pthread_mutex_lock(&params->lock_fork);
		params->forks[first_take] = 1;
		pthread_mutex_unlock(&params->lock_fork);
		return (SUCCESS);
	}
	return (FAILURE);
//	printf(" #DEGUB[%zu]take    %zu, %zu\n", idx, first_take, second_take);
}

int	take_right_forks(t_params *params, size_t idx)
{
	const size_t	num_of_philos = params->num_of_philos;
	size_t			second_take;

	second_take = (idx + 1) % num_of_philos;
	if (idx % 2 == 1)
		second_take = idx % num_of_philos;
	if (params->forks[second_take] == 0)
	{
		pthread_mutex_lock(&params->lock_fork);
		params->forks[second_take] = 1;
		pthread_mutex_unlock(&params->lock_fork);
		return (SUCCESS);
	}
	return (FAILURE);
}


int	release_forks(t_params *params, size_t idx)
{
	const size_t	num_of_philos = params->num_of_philos;
	size_t			first_take;
	size_t			second_take;

	first_take = idx % num_of_philos;
	second_take = (idx + 1) % num_of_philos;
	if (idx % 2 == 1)
	{
		first_take = (idx + 1) % num_of_philos;
		second_take = idx % num_of_philos;
	}
	pthread_mutex_lock(&params->lock_fork);
	params->forks[first_take] = 0;
	params->forks[second_take] = 0;
	pthread_mutex_unlock(&params->lock_fork);
	return (SUCCESS);
}

bool	is_died(time_t start, time_t now, time_t time_to_die)
{
	return (now - start >= time_to_die);
}

// 待っている間のdied判定


void	*do_routine(void *v_philo)
{
	t_each_philo		*philo;
	t_params	*params;
	size_t		idx;
	time_t		now_time;

	philo = (t_each_philo *)v_philo;
	params = philo->params;
	idx = philo->idx;
	while (!params->is_died)
	{
		while (take_forks(params, idx) == FAILURE)
		{
			now_time = get_unix_time_ms();
			if (is_died(philo->std_time, now_time, params->time_to_die))
			{
				pthread_mutex_lock(&params->lock_died);
				params->is_died = true;
				if (params->died_philo == -1)
					params->died_philo = (ssize_t)idx;
				pthread_mutex_unlock(&params->lock_died);

				pthread_mutex_lock(&params->lock_print);
				print_msg(idx, TYPE_DIED, philo->std_time + params->time_to_die, params);
				pthread_mutex_unlock(&params->lock_print);
				return (NULL);
			}
			if (params->is_died || is_finished(params))
				return (NULL);
		}
		now_time = get_unix_time_ms();
		if (is_died(philo->std_time, now_time, params->time_to_die))
		{
			pthread_mutex_lock(&params->lock_died);
			params->is_died = true;
			if (params->died_philo == -1)
				params->died_philo = (ssize_t)idx;
			pthread_mutex_unlock(&params->lock_died);

			pthread_mutex_lock(&params->lock_print);
			print_msg(idx, TYPE_DIED, philo->std_time + params->time_to_die, params);
			pthread_mutex_unlock(&params->lock_print);
			return (NULL);
		}
		if (params->is_died || is_finished(params))
			return (NULL);

		philo->std_time = now_time;
		pthread_mutex_lock(&params->lock_print);
		print_msg(idx, TYPE_FORK, now_time, params);
		pthread_mutex_unlock(&params->lock_print);

		// eat & print
		if (params->is_died || is_finished(params))
			return (NULL);
		pthread_mutex_lock(&params->lock_print);
		print_msg(idx, TYPE_EATING, get_unix_time_ms(), params);
		pthread_mutex_unlock(&params->lock_print);
		usleep(params->time_to_eat * 1000);

		// release lock_fork
		release_forks(params, idx);
		params->eat_times[idx]++;

		// sleep & print
		if (params->is_died || is_finished(params))
			return (NULL);
		pthread_mutex_lock(&params->lock_print);
		print_msg(idx, TYPE_SLEEPING, get_unix_time_ms(), params);
		pthread_mutex_unlock(&params->lock_print);
		usleep(params->time_to_sleep * 1000);

		// think & print

		if (params->is_died || is_meet_mast_eat_time(params))
			return (NULL);
		pthread_mutex_lock(&params->lock_print);
		print_msg(idx, TYPE_THINKING, get_unix_time_ms(), params);
		pthread_mutex_unlock(&params->lock_print);
	}
	return (NULL);
}

int	monitor_philos(t_params *params)
{
	if (!params)
		return (FAILURE);

	return (SUCCESS);
}

*/