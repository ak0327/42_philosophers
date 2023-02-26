/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/25 16:15:23 by takira            #+#    #+#             */
/*   Updated: 2023/02/26 19:35:43 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

static int	eating(t_philo_info *philo)
{
	t_info	*info;

	info = philo->info_ptr;
	if (print_msg_if_alive(philo, TYPE_EATING) == PHILO_DIED)
		return (PHILO_DIED);
	if (sem_wait(philo->sem_philo) != SUCCESS)
		return (PROCESS_ERROR);
	philo->start_time = get_unix_time_ms();
	if (sem_post(philo->sem_philo) != SUCCESS)
		return (PROCESS_ERROR);
	sleep_ms(info->time_to_eat);
	return (SUCCESS);
}

static int	sleeping_and_thinking(t_philo_info *philo)
{
	t_info	*info;

	info = philo->info_ptr;
	if (print_msg_if_alive(philo, TYPE_SLEEPING) == PHILO_DIED)
		return (PHILO_DIED);
	sleep_ms(info->time_to_sleep);
	if (print_msg_if_alive(philo, TYPE_THINKING) == PHILO_DIED)
		return (PHILO_DIED);
	return (SUCCESS);
}

void	*routine(t_philo_info *philo)
{
	int		ret_value;

	while (true)
	{
		ret_value = take_forks(philo);
		if (ret_value != SUCCESS)
			break ;
		ret_value = eating(philo);
		if (ret_value != SUCCESS)
			break ;
		ret_value = put_forks_and_update_eat_times(philo);
		if (ret_value != SUCCESS)
			break ;
		ret_value = sleeping_and_thinking(philo);
		if (ret_value != SUCCESS)
			break ;
		ret_value = check_continue(philo);
		if (ret_value != CONTINUE)
			break ;
	}
	if (ret_value == PHILO_DIED)
		return ((void *)PHILO_DIED);
	if (ret_value == PROCESS_ERROR)
		return ((void *)PROCESS_ERROR);
	return ((void *)SUCCESS);
}
