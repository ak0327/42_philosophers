/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/17 13:12:09 by takira            #+#    #+#             */
/*   Updated: 2023/02/22 19:25:45 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	start_eating(t_philo_info *philo, int prev_ret_val)
{
	int				ret_value;
	const time_t	time = get_unix_time_ms();

	if (prev_ret_val != SUCCESS)
		return (prev_ret_val);
	ret_value = print_msg(philo->idx, TYPE_EATING, philo->params_ptr, time);
	if (ret_value == PHILO_DIED)
		return (PHILO_DIED);
	if (pthread_mutex_lock(&philo->philo_mutex) != SUCCESS)
		return (PROCESS_ERROR);
	philo->start_time = time;
	pthread_mutex_unlock(&philo->philo_mutex);

	usleep(philo->params_ptr->time_to_eat * 1000);
	return (ret_value);
}

static int	start_sleeping(t_philo_info *philo, int prev_ret_val)
{
	int				ret_value;
	const time_t	time = get_unix_time_ms();

	if (prev_ret_val != SUCCESS)
		return (prev_ret_val);

	ret_value = print_msg(philo->idx, TYPE_SLEEPING, philo->params_ptr, time);
	if (ret_value == PHILO_DIED)
		return (PHILO_DIED);

	usleep(philo->params_ptr->time_to_sleep * 1000);
	return (ret_value);
}

static int	start_thinking(t_philo_info *philo, int prev_ret_val)
{
	int				ret_value;
	const time_t	time = get_unix_time_ms();

	if (prev_ret_val != SUCCESS)
		return (prev_ret_val);

	ret_value = print_msg(philo->idx, TYPE_THINKING, philo->params_ptr, time);
	return (ret_value);
}

static int	update_eat_times(t_philo_info *philo, int prev_ret_val)
{
	size_t	eat_times;

	if (prev_ret_val != SUCCESS && prev_ret_val != PHILO_ALIVE)
		return (prev_ret_val);

	if (philo->params_ptr->must_eat_times < 0)
		return (SUCCESS);

	if (pthread_mutex_lock(&philo->philo_mutex) != SUCCESS)
		return (PROCESS_ERROR);
	philo->eat_times++;
	if (pthread_mutex_unlock(&philo->philo_mutex) != SUCCESS)
		return (PROCESS_ERROR);

	eat_times = philo->eat_times;
	if ((ssize_t)eat_times >= philo->params_ptr->must_eat_times)
		philo->is_meet_eat_times = false;

	return (SUCCESS);
}

void	*routine(void *v_philo_info)
{
	t_philo_info	*philo;
	int				ret_value;

	ret_value = SUCCESS;
	philo = (t_philo_info *)v_philo_info;
	while (ret_value == SUCCESS)
	{
		ret_value = take_forks(philo);
		if (ret_value == CONTINUE)
			continue ;
		ret_value = start_eating(philo, ret_value);
		ret_value = put_forks(philo, ret_value);
		ret_value = update_eat_times(philo, ret_value);
		ret_value = start_sleeping(philo, ret_value);
		ret_value = start_thinking(philo, ret_value);
		ret_value = get_is_died(philo->params_ptr, NULL, ret_value);
		ret_value = is_meet_must_eat_times(philo->params_ptr, ret_value);
	}
	if (ret_value == PROCESS_ERROR)
		printf("[Error] Process abort\n");
	return (NULL);
}

