/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/17 13:12:09 by takira            #+#    #+#             */
/*   Updated: 2023/02/18 09:59:41 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	waiting(t_each_philo *philo, t_params *params)
{


}

void	*do_routine(void *v_philo)
{
	t_each_philo	*philo;
	t_params		*params;
	time_t			now_time;

	philo = (t_each_philo *)v_philo;
	params = philo->params;
	while (true)
	{
		// waiting
		if (waiting(philo, params) == INTERRUPT)
			return (NULL);

		// print taken a fork x 2

		// eat

		// release forks

		// sleep

		// think
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

bool	is_finished(t_params *params)
{
	const ssize_t	must_eat_times = params->must_eat_times;
	size_t	idx;

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
			if (is_died(philo->start_time, now_time, params->time_to_die))
			{
				pthread_mutex_lock(&params->lock_died);
				params->is_died = true;
				if (params->died_philo == -1)
					params->died_philo = (ssize_t)idx;
				pthread_mutex_unlock(&params->lock_died);

				pthread_mutex_lock(&params->lock_print);
				print_msg(idx, TYPE_DIED, philo->start_time + params->time_to_die, params);
				pthread_mutex_unlock(&params->lock_print);
				return (NULL);
			}
			if (params->is_died || is_finished(params))
				return (NULL);
		}
		now_time = get_unix_time_ms();
		if (is_died(philo->start_time, now_time, params->time_to_die))
		{
			pthread_mutex_lock(&params->lock_died);
			params->is_died = true;
			if (params->died_philo == -1)
				params->died_philo = (ssize_t)idx;
			pthread_mutex_unlock(&params->lock_died);

			pthread_mutex_lock(&params->lock_print);
			print_msg(idx, TYPE_DIED, philo->start_time + params->time_to_die, params);
			pthread_mutex_unlock(&params->lock_print);
			return (NULL);
		}
		if (params->is_died || is_finished(params))
			return (NULL);

		philo->start_time = now_time;
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

		if (params->is_died || is_finished(params))
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