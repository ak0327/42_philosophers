/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/17 13:12:09 by takira            #+#    #+#             */
/*   Updated: 2023/02/22 13:33:43 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	start_eating(t_philo_info *philo)
{
	bool			is_died;

	is_died = get_is_died(philo->params_ptr, NULL);
	if (is_died)
		return ;

	print_msg(philo->idx, TYPE_EATING, philo->params_ptr);

	pthread_mutex_lock(&philo->philo_mutex);
	philo->start_time = get_unix_time_ms();
	pthread_mutex_unlock(&philo->philo_mutex);

	usleep(philo->params_ptr->time_to_eat * 1000);
}

static void	start_sleeping(t_philo_info *philo)
{
	bool			is_died;

	is_died = get_is_died(philo->params_ptr, NULL);
	if (is_died)
		return ;
//	params->state[philo->idx] = STATE_SLEEPING;
	print_msg(philo->idx, TYPE_SLEEPING, philo->params_ptr);
	usleep(philo->params_ptr->time_to_sleep * 1000);
}

static void	start_thinking(t_philo_info *philo)
{
	bool			is_died;

	is_died = get_is_died(philo->params_ptr, NULL);
	if (is_died)
		return ;
//	params->state[philo->idx] = STATE_THINKING;
	print_msg(philo->idx, TYPE_THINKING, philo->params_ptr);
}

static void	update_eat_times(t_philo_info *philo)
{
	size_t	eat_times;

	if (philo->params_ptr->must_eat_times < 0)
		return ;

	pthread_mutex_lock(&philo->philo_mutex);
	philo->eat_times++;
	pthread_mutex_unlock(&philo->philo_mutex);

	eat_times = philo->eat_times;
	if ((ssize_t)eat_times >= philo->params_ptr->must_eat_times)
		philo->is_meet_eat_times = false;
}

void	*routine(void *v_philo_info)
{
	t_philo_info	*philo;

	philo = (t_philo_info *)v_philo_info;
//	printf("start:%zu\n", philo->idx);
	while (true)
	{
		take_forks(philo);
		start_eating(philo);
		put_forks(philo);
		update_eat_times(philo);
		start_sleeping(philo);
		start_thinking(philo);
		if (is_meet_must_eat_times(philo->params_ptr))
			break ;
		if (get_is_died(philo->params_ptr, NULL))
			break ;
	}
	return (NULL);
}

