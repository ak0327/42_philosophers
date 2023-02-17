/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/17 13:12:09 by takira            #+#    #+#             */
/*   Updated: 2023/02/17 23:25:04 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <sys/time.h>

time_t	get_unix_time_ms(void)
{
	struct timeval	tv;
	time_t			ret_time;

	gettimeofday(&tv, NULL);
	ret_time = tv.tv_sec * 1000
			+ tv.tv_usec / 1000;
	return (ret_time);
}

//void	print_timestamp(void)
//{
//	struct timeval	tv;
//	t_time			time;
//
//	gettimeofday(&tv, NULL);
//	time.unix_time = tv.tv_sec;
//	time.msec = tv.tv_usec/1000;
//	time.sec = (int)(time.unix_time % 60);
//	time.min = (int)((time.unix_time / 60) % 60);
//	time.hour = (int)((time.unix_time / 60 / 60 + JST) % 24);
//	printf("%02d:%02d:%02d %03d",time.hour, time.min, time.sec, time.msec);
//}

char	*get_print_msg(t_print_type type)
{
	if (type == TYPE_FORK)
		return (PRINT_FORK);
	if (type == TYPE_EATING)
		return (PRINT_EATING);
	if (type == TYPE_SLEEPING)
		return (PRINT_SLEEPING);
	if (type == TYPE_THINKING)
		return (PRINT_THINKING);
	return (PRINT_DIED);
}

void	print_timestamp(void)
{
	struct timeval	tv;
	time_t			unix_time;
	time_t			msec;

	gettimeofday(&tv, NULL);
	unix_time = tv.tv_sec;
	msec = tv.tv_usec/1000;
	printf("%ld:%03d(ms)",unix_time, (int)msec);
}

void	print_msg(size_t idx, t_print_type type, time_t time, t_params *params)
{
	const time_t	unix_time_sec = time / 1000;
	const time_t	unix_time_msec = time % 1000;
	const char		*msg = get_print_msg(type);

	if (!params->is_died || params->died_philo == (ssize_t)idx)
	{
	//	printf("[%zu] %ld:%03ld(ms) %03zu %s\n", idx, unix_time_sec, unix_time_msec, idx, msg);
		printf("%ld%03ld %zu %s\n", unix_time_sec, unix_time_msec, idx, msg);
	}
}

// idx=0,	 -> 0, 1
// idx=1,	 -> 1, 2
// idx=len-1 -> len-1, 0

int	take_forks(t_params *params, size_t	idx)
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
	if (params->wait_philo != -1 && (size_t)params->wait_philo == idx)
	{
		params->wait_philo++;
		params->wait_philo %= (ssize_t)params->num_of_philos - 1;
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
//	printf(" #DEGUB[%zu]release %zu, %zu\n", idx, first_take, second_take);
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
	t_philo		*philo;
	t_params	*params;
	size_t		philo_idx;
	time_t		now_time;

	philo = (t_philo *)v_philo;
	params = philo->params;
	philo_idx = philo->philo_idx;
	while (!params->is_died)
	{
		while (take_forks(params, philo_idx) == FAILURE)
		{
			now_time = get_unix_time_ms();
			if (is_died(philo->start_time, now_time, params->time_to_die))
			{
				pthread_mutex_lock(&params->lock_died);
				params->is_died = true;
				if (params->died_philo == -1)
					params->died_philo = (ssize_t)philo_idx;
				pthread_mutex_unlock(&params->lock_died);

				pthread_mutex_lock(&params->lock_print);
				print_msg(philo_idx, TYPE_DIED, philo->start_time + params->time_to_die, params);
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
				params->died_philo = (ssize_t)philo_idx;
			pthread_mutex_unlock(&params->lock_died);

			pthread_mutex_lock(&params->lock_print);
			print_msg(philo_idx, TYPE_DIED, philo->start_time + params->time_to_die, params);
			pthread_mutex_unlock(&params->lock_print);
			return (NULL);
		}
		if (params->is_died || is_finished(params))
			return (NULL);

		philo->start_time = now_time;
		pthread_mutex_lock(&params->lock_print);
		print_msg(philo_idx, TYPE_FORK, now_time, params);
		pthread_mutex_unlock(&params->lock_print);

		// eat & print
		if (params->is_died || is_finished(params))
			return (NULL);
		pthread_mutex_lock(&params->lock_print);
		print_msg(philo_idx, TYPE_EATING, get_unix_time_ms(), params);
		pthread_mutex_unlock(&params->lock_print);
		usleep(params->time_to_eat * 1000);

		// release lock_fork
		release_forks(params, philo_idx);
		params->eat_times[philo_idx]++;

		// sleep & print
		if (params->is_died || is_finished(params))
			return (NULL);
		pthread_mutex_lock(&params->lock_print);
		print_msg(philo_idx, TYPE_SLEEPING, get_unix_time_ms(), params);
		pthread_mutex_unlock(&params->lock_print);
		usleep(params->time_to_sleep * 1000);

		// think & print

		if (params->is_died || is_finished(params))
			return (NULL);
		pthread_mutex_lock(&params->lock_print);
		print_msg(philo_idx, TYPE_THINKING, get_unix_time_ms(), params);
		pthread_mutex_unlock(&params->lock_print);
	}
	return (NULL);
}

int	create_threads(t_params *params, t_philo *philo)
{
	size_t			idx;
	const time_t	start_time = get_unix_time_ms();

	if (!params)
		return (FAILURE);
	idx = 0;
	while (idx < params->num_of_philos)
	{
		philo->start_time = start_time;
		if (pthread_create(&params->philo_no[idx], NULL, do_routine, (void *)philo) != SUCCESS)
			return (PROCESS_ERROR);
		philo = philo->next;
		idx++;
	}
	return (SUCCESS);
}

int	monitor_philos(t_params *params)
{
	if (!params)
		return (FAILURE);

	return (SUCCESS);
}

int	terminate_threads(t_params *params)
{
	size_t	idx;

	idx = 0;
	while (idx < params->num_of_philos)
	{
		if (pthread_join(params->philo_no[idx], NULL) != SUCCESS)
			return (PROCESS_ERROR);
		idx++;
	}
	return (SUCCESS);
}
