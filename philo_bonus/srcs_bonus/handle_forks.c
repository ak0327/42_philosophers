/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_forks.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/26 16:06:22 by takira            #+#    #+#             */
/*   Updated: 2023/02/26 20:25:18 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

static void	wait_while_philo_alive(t_philo_info *philo)
{
	while (true)
	{
		if (get_is_died(philo, NULL) == true)
			break ;
		usleep(500);
	}
}

int	take_forks(t_philo_info *philo)
{
	if (sem_wait(philo->info_ptr->sem_waiter) != SUCCESS)
		return (PROCESS_ERROR);
	if (sem_wait(philo->info_ptr->sem_forks) != SUCCESS)
		return (PROCESS_ERROR);
	if (print_msg_consider_died(philo, TYPE_FORK) == PHILO_DIED)
		return (PHILO_DIED);
	if (philo->info_ptr->num_of_philos == 1)
	{
		wait_while_philo_alive(philo);
		print_msg(TYPE_DIED, philo);
		return (PHILO_DIED);
	}
	if (sem_wait(philo->info_ptr->sem_forks) != SUCCESS)
		return (PROCESS_ERROR);
	if (print_msg_consider_died(philo, TYPE_FORK) == PHILO_DIED)
		return (PHILO_DIED);
	if (sem_post(philo->info_ptr->sem_waiter) != SUCCESS)
		return (PROCESS_ERROR);
	return (SUCCESS);
}

int	put_forks_and_update_eat_times(t_philo_info *philo)
{
	t_info	*info;

	info = philo->info_ptr;
	if (sem_post(info->sem_forks) != SUCCESS)
		return (PROCESS_ERROR);
	if (sem_post(info->sem_forks) != SUCCESS)
		return (PROCESS_ERROR);
	if (sem_wait(philo->sem_philo) != SUCCESS)
		return (PROCESS_ERROR);
	philo->eat_cnt++;
	if (info->must_eat_times > 0 \
	&& (ssize_t)philo->eat_cnt >= info->must_eat_times)
		philo->is_satisfied = true;
	if (sem_post(philo->sem_philo) != SUCCESS)
		return (PROCESS_ERROR);
	return (SUCCESS);
}
