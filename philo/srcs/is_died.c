/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_died.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/22 23:11:00 by takira            #+#    #+#             */
/*   Updated: 2023/02/24 14:58:18 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static  int	check_and_update_died_wo_lock(\
t_params *params, size_t idx, time_t now_time)
{
	time_t	start_time;
	time_t	delta_time;
	bool	is_meet_eat_times;

//	printf("   #check_and_update_died-1\n");
	is_meet_eat_times = get_is_meet_eat_times(&params->philo_info[idx]);
	start_time = get_start_time(&params->philo_info[idx]);
	delta_time = get_delta_time_ms(now_time, start_time);

//	printf("   #check_and_update_died-2\n");
	if (!is_meet_eat_times && delta_time >= params->time_to_die)
	{
//		printf("   #check_and_update_died-3(died)\n");
		params->is_died = PHILO_DIED;
		params->died_idx = (ssize_t)idx;
//		print_msg(idx, TYPE_DIED, params);
//		params->died_idx = -1;
		return (PHILO_DIED);
	}
//	printf("   #check_and_update_died-3(alive)\n");

	return (PHILO_ALIVE);
}

int	check_and_update_died(t_params *params, size_t idx, time_t now_time)
{
	int		ret_value;
	int		is_died;

	ret_value = PHILO_DIED;
//	printf("#check_died-1\n");
	if (pthread_mutex_lock(&params->died_mutex) != SUCCESS)
		return (PROCESS_ERROR);
//	printf("#check_died-2\n");
	is_died = params->is_died;
	if (is_died == PHILO_ALIVE)
		ret_value = check_and_update_died_wo_lock(params, idx, now_time);
//	printf("#check_died-3\n");
	if (pthread_mutex_unlock(&params->died_mutex) != SUCCESS)
		return (PROCESS_ERROR);
//	printf("#check_died-4, ret:%d\n", ret_value);
	return (ret_value);
}

int	is_some_philo_died(t_params *params)
{
	const time_t	now_time = get_unix_time_ms();
	size_t			idx;
	int				ret_val;

	idx = 0;
//	printf(" #is_some_philo_died-1 *params:%p\n", params);
	if (pthread_mutex_lock(&params->died_mutex) != SUCCESS)
	{
//		printf(" #is_some_philo_died-error -1\n");
		return (PROCESS_ERROR);
	}
//	printf(" #is_some_philo_died-2\n");
	ret_val = params->is_died;
	while (ret_val == PHILO_ALIVE && idx < params->num_of_philos)
	{
//		printf(" #is_some_philo_died-3\n");
		ret_val = check_and_update_died_wo_lock(params, idx, now_time);
		if (ret_val != SUCCESS)
			break ;
		idx++;
	}
//	printf(" #is_some_philo_died-4\n");
	if (pthread_mutex_unlock(&params->died_mutex) != SUCCESS)
		return (PROCESS_ERROR);
//	printf(" #is_some_philo_died-5 (ret:%d)\n", ret_val);
	return (ret_val);
}

int	get_is_died(t_params *params, ssize_t *idx, int prev_ret_value)
{
	int	is_died;

	if (!(prev_ret_value == SUCCESS || prev_ret_value == CONTINUE))
		return (prev_ret_value);
	if (pthread_mutex_lock(&params->died_mutex) != SUCCESS)
		return (PROCESS_ERROR);
	is_died = params->is_died;
	if (idx)
		*idx = params->died_idx;
	if (pthread_mutex_unlock(&params->died_mutex) != SUCCESS)
		return (PROCESS_ERROR);
	return (is_died);
}
