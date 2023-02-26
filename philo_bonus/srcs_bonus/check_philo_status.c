/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_philo_status.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/26 16:09:17 by takira            #+#    #+#             */
/*   Updated: 2023/02/26 20:24:29 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

bool	get_is_died(t_philo_info *philo, bool *is_died_printed)
{
	bool	is_died;

	if (sem_wait(philo->sem_philo) != SUCCESS)
		return (PROCESS_ERROR);
	is_died = philo->is_died;
	if (is_died_printed)
		*is_died_printed = philo->is_died_printed;
	if (sem_post(philo->sem_philo) != SUCCESS)
		return (PROCESS_ERROR);
	return (is_died);
}

size_t	get_eat_cnt(t_philo_info *philo)
{
	size_t	eat_cnt;

	if (sem_wait(philo->sem_philo) != SUCCESS)
		return (PROCESS_ERROR);
	eat_cnt = philo->eat_cnt;
	if (sem_post(philo->sem_philo) != SUCCESS)
		return (PROCESS_ERROR);
	return (eat_cnt);
}

int	check_philo_died(t_philo_info *philo, time_t now_time)
{
	int		ret_value;
	time_t	start_time;
	time_t	delta_time;

	ret_value = PHILO_ALIVE;
	if (sem_wait(philo->sem_philo) != SUCCESS)
		return (PROCESS_ERROR);
	start_time = philo->start_time;
	delta_time = get_delta_time_ms(now_time, start_time);
	if (!philo->is_died && delta_time >= philo->info_ptr->time_to_die)
	{
		philo->is_died = PHILO_DIED;
		ret_value = PHILO_DIED;
		print_msg(TYPE_DIED, philo);
		philo->is_died_printed = true;
	}
	if (sem_post(philo->sem_philo) != SUCCESS)
		return (PROCESS_ERROR);
	return (ret_value);
}

int	check_continue(t_philo_info *philo)
{
	bool	is_satisfied;
	bool	is_died;

	if (sem_wait(philo->sem_philo) != SUCCESS)
		return (PROCESS_ERROR);
	is_died = philo->is_died;
	is_satisfied = philo->is_satisfied;
	if (sem_post(philo->sem_philo) != SUCCESS)
		return (PROCESS_ERROR);
	if (is_died)
		return (PHILO_DIED);
	if (is_satisfied)
		return (BREAK);
	return (CONTINUE);
}
