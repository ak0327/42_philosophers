/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/17 13:12:09 by takira            #+#    #+#             */
/*   Updated: 2023/02/24 16:01:02 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	start_eating(t_philo_info *philo)
{
	int				ret_value;
	const time_t	now_time = get_unix_time_ms();

	if (check_and_update_died(\
	philo->params_ptr, philo->idx, now_time) == PHILO_DIED)
		return (PHILO_DIED);
	ret_value = print_msg(philo->idx, TYPE_EATING, philo->params_ptr);
	if (ret_value == PHILO_DIED)
		return (PHILO_DIED);
	if (pthread_mutex_lock(&philo->philo_mutex) != SUCCESS)
		return (PROCESS_ERROR);
	philo->start_time = now_time;
	if (pthread_mutex_unlock(&philo->philo_mutex) != SUCCESS)
		return (PROCESS_ERROR);
	sleep_ms(philo->params_ptr->time_to_eat);
	return (ret_value);
}

static int	start_sleeping(t_philo_info *philo, int prev_ret_val)
{
	int				ret_value;
	const time_t	now_time = get_unix_time_ms();

	if (prev_ret_val != SUCCESS)
		return (prev_ret_val);
	if (check_and_update_died(\
	philo->params_ptr, philo->idx, now_time) == PHILO_DIED)
		return (PHILO_DIED);
	ret_value = print_msg(philo->idx, TYPE_SLEEPING, philo->params_ptr);
	if (ret_value == PHILO_DIED)
		return (PHILO_DIED);
	sleep_ms(philo->params_ptr->time_to_sleep);
	return (ret_value);
}

static int	start_thinking(t_philo_info *philo, int prev_ret_val)
{
	int				ret_value;
	const time_t	now_time = get_unix_time_ms();

	if (prev_ret_val != SUCCESS)
		return (prev_ret_val);
	if (check_and_update_died(\
	philo->params_ptr, philo->idx, now_time) == PHILO_DIED)
		return (PHILO_DIED);
	ret_value = print_msg(philo->idx, TYPE_THINKING, philo->params_ptr);
	return (ret_value);
}

static int	update_eat_times(t_philo_info *philo, int prev_ret_val)
{
	if (prev_ret_val != SUCCESS)
		return (prev_ret_val);
	if (philo->params_ptr->must_eat_times < 0)
		return (SUCCESS);
	if (pthread_mutex_lock(&philo->philo_mutex) != SUCCESS)
		return (PROCESS_ERROR);
	philo->eat_times++;
	if ((ssize_t)philo->eat_times >= philo->params_ptr->must_eat_times)
		philo->is_meet_eat_times = true;
	if (pthread_mutex_unlock(&philo->philo_mutex) != SUCCESS)
		return (PROCESS_ERROR);
	return (SUCCESS);
}

void	*routine(void *v_philo_info)
{
	t_philo_info	*philo;
	int				ret_value;
	ssize_t			died_philo;

	ret_value = SUCCESS;
	philo = (t_philo_info *)v_philo_info;
	while (ret_value == SUCCESS || ret_value == CONTINUE)
	{
		if (take_forks(philo) != SUCCESS)
			continue ;
		ret_value = start_eating(philo);
		ret_value = put_forks(philo, ret_value);
		ret_value = update_eat_times(philo, ret_value);
		ret_value = start_sleeping(philo, ret_value);
		ret_value = start_thinking(philo, ret_value);
		ret_value = get_is_died(philo->params_ptr, NULL, ret_value);
		if (ret_value == PHILO_DIED || get_is_meet_must_eat_times(philo))
			break ;
	}
	if (ret_value == PROCESS_ERROR)
		printf("[Error] Process abort\n");
	get_is_died(philo->params_ptr, &died_philo, SUCCESS);
	if (ret_value == PHILO_DIED && died_philo == (ssize_t)philo->idx)
		print_msg(philo->idx, TYPE_DIED, philo->params_ptr);
	return (NULL);
}
