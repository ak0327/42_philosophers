/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/17 13:12:09 by takira            #+#    #+#             */
/*   Updated: 2023/02/21 13:02:29 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	print_eat_times(t_params *params)
{
	size_t idx;

	idx = 0;
	printf("eat times:");
	while (idx < params->num_of_philos)
	{
		printf("%zu", params->each_eat_times[idx]);
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
		return("thinking");
	if (state == STATE_HUNGRY)
		return("\x1b[33mHUNGRY\x1b[0m  ");
	if (state == STATE_WAITING)
		return("\x1b[33mWAITING\x1b[0m  ");
	if (state == STATE_EATING)
		return("eating  ");
	if (state == STATE_SLEEPING)
		return("sleeping");
	return("\x1b[31mDIED\x1b[0m    ");
}

void	debug_print_state_wo_lock(t_params *params, size_t id)
{
	size_t	idx;
	char	*str;

	idx = 0;
	pthread_mutex_lock(&params->lock_print);

	printf("# state wo(%zu):", id);
	print_timestamp();
	printf(":");
	while (idx < params->num_of_philos)
	{
		str = get_state_str(params->state[idx]);
		printf("[%zu](%zu)%s", idx, params->each_eat_times[idx], str);
		idx++;
		if (idx < params->num_of_philos)
			printf(", ");
		else
			printf("\n");
	}
	printf("\n");
	pthread_mutex_unlock(&params->lock_print);

}

void	debug_print_state_w_lock(t_params *params, size_t id)
{
	pthread_mutex_lock(&params->lock_state);

	debug_print_state_wo_lock(params, id);

	pthread_mutex_unlock(&params->lock_state);

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

	if (must_eat_times == -1)
		return (false);
	idx = 0;
	while (idx < params->num_of_philos)
	{
		if (params->each_eat_times[idx] < (size_t)params->must_eat_times)
			return (false);
		idx++;
	}
	return (true);
}


int	check_philo_alive(t_params *params, size_t idx, time_t std_time)
{
	if (params->is_died && params->died_philo != (ssize_t)idx)
	{
		printf("ret PHILO_DIED died:%zu (%zu)\n", params->died_philo, idx);
		return (PHILO_DIED);
	}

	if (check_died(params, idx))
	{
//		printf("check_died = true (%zu)-1\n", idx);
		pthread_mutex_lock(&params->lock_died);
//		printf("check_died = true (%zu)-2\n", idx);

		if (!params->is_died && params->died_philo == -1)
		{
//			printf("check_died = true (%zu)-3\n", idx);

			params->is_died = true;
			params->died_philo = (ssize_t)idx;
			pthread_mutex_unlock(&params->lock_died);

			pthread_mutex_lock(&params->lock_state);
			params->state[idx] = STATE_DIED;
			pthread_mutex_unlock(&params->lock_state);
		}
//		printf("check_died = true (%zu)-4\n", idx);

		pthread_mutex_lock(&params->lock_print);
		print_msg(idx, TYPE_DIED, std_time + params->time_to_die, params);
		pthread_mutex_unlock(&params->lock_print);
		printf("\x1b[31mdied dt:%ld\n\x1b[0m", get_unix_time_ms() - std_time);

		debug_print_state_wo_lock(params, idx);
		return (PHILO_DIED);
	}

	return (PHILO_ALIVE);
}

void	*routine(void *v_philo)
{
	t_philo_info	*philo;
	t_params		*params;

	philo = (t_philo_info *)v_philo;
	params = philo->params;

//	debug_print_state_w_lock(params, philo->idx);
	while (is_meet_mast_eat_times(params) == false)
	{

		/* take a fork */
//		debug_print_state_w_lock(params, philo->idx);

		pthread_mutex_lock(&params->lock_state);
		params->state[philo->idx] = take_forks_wo_lock_state(params, philo); //...(1)
		pthread_mutex_unlock(&params->lock_state);


		// (1) でEATINGにできなかった場合
		// 両隣のforkが使える状態になればSTATE_EATINGになる

		if (params->state[philo->idx] != STATE_EATING)
		{
			if (check_philo_alive(params, philo->idx, philo->start_time) ==	PHILO_DIED)
			{
				printf("ret PHILO_DIED died:%zu (%zu)\n", params->died_philo, philo->idx);
				return (NULL);
			}
			continue;
		}

		debug_print_state_w_lock(params, philo->idx);

	/*
		while (params->state[philo->idx] == STATE_WAITING) //wait
		{
			if (check_philo_alive(params, philo->idx, philo->start_time) == PHILO_DIED)
			{
//				printf("ret NULL because %zu died (%zu)\n", params->died_philo, philo->idx);
//				pthread_mutex_lock(&params->lock_state);
//				params->state[philo->idx] = put_forks_wo_lock_state(params, philo);
//				pthread_mutex_unlock(&params->lock_state);
				return (NULL);
			}

//			usleep(500);
		}
*/
//		debug_print_state_w_lock(params, philo->idx);

//		take_forks_wo_lock_state(params, philo);
		if (check_philo_alive(params, philo->idx, philo->start_time) == PHILO_DIED)
		{
//			printf("put because %zu died (%zu)\n", params->died_philo, philo->idx);
			pthread_mutex_lock(&params->lock_state);
			put_forks_wo_lock_state(params, philo);
			pthread_mutex_unlock(&params->lock_state);
//			printf("after put ret NULL (%zu)\n", philo->idx);
			return (NULL);
		}

		/* eat */
		pthread_mutex_lock(&params->lock_print);
		philo->start_time = get_unix_time_ms();
		print_msg(philo->idx, TYPE_FORK, philo->start_time, params);
		print_msg(philo->idx, TYPE_FORK, philo->start_time, params);
		print_msg(philo->idx, TYPE_EATING, philo->start_time, params);
		pthread_mutex_unlock(&params->lock_print);

		usleep(params->time_to_eat * 1000);

		/* put fork_arr */
		if (check_philo_alive(params, philo->idx, philo->start_time) == PHILO_DIED)
		{
//			printf("put because %zu died (%zu)\n", params->died_philo, philo->idx);
			pthread_mutex_lock(&params->lock_state);
			put_forks_wo_lock_state(params, philo);
			pthread_mutex_unlock(&params->lock_state);
//			printf("after put ret NULL (%zu)\n", philo->idx);
			return (NULL);
		}

		pthread_mutex_lock(&params->lock_state);
		put_forks_wo_lock_state(params, philo);
		pthread_mutex_unlock(&params->lock_state);

		pthread_mutex_lock(&params->lock_eat_times);
		params->each_eat_times[philo->idx]++;
		pthread_mutex_unlock(&params->lock_eat_times);

//		debug_print_state_w_lock(params, philo->idx);

		/* sleep */
		if (check_philo_alive(params, philo->idx, philo->start_time) == PHILO_DIED)
			return (NULL);

		pthread_mutex_lock(&params->lock_print);
		print_msg(philo->idx, TYPE_SLEEPING, get_unix_time_ms(), params);
		pthread_mutex_unlock(&params->lock_print);

		usleep(params->time_to_sleep * 1000);
		/* think */
		if (check_philo_alive(params, philo->idx, philo->start_time) == PHILO_DIED)
			return (NULL);

		pthread_mutex_lock(&params->lock_state);
		params->state[philo->idx] = STATE_THINKING;
		pthread_mutex_unlock(&params->lock_state);

		pthread_mutex_lock(&params->lock_print);
		print_msg(philo->idx, TYPE_THINKING, get_unix_time_ms(), params);
		pthread_mutex_unlock(&params->lock_print);

	}
	return (NULL);
}
