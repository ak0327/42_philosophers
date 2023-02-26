/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_forks.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/26 16:06:22 by takira            #+#    #+#             */
/*   Updated: 2023/02/26 16:10:29 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "philo_bonus.h"

int	take_forks(t_philo_info *philo)
{
	t_info	*info;
	time_t	now_time;

	info = philo->info_ptr;
//	printf("  (%zu)-1 waiter wait\n", philo->idx + 1);
	if (sem_wait(info->sem_waiter) != SUCCESS)
		return (PROCESS_ERROR);

//	printf("  (%zu)-2 fork wait\n", philo->idx + 1);
	if (sem_wait(info->sem_forks) != SUCCESS)
		return (PROCESS_ERROR);

	now_time = get_unix_time_ms();
	if (check_philo_died(philo, now_time) == PHILO_DIED)
	{
		print_msg(TYPE_DIED, philo);
		sem_post(info->sem_waiter);
//		printf("  (%zu)-3 died\n", philo->idx + 1);
		return (PHILO_DIED);
	}
	print_msg(TYPE_FORK, philo);

//	printf("  (%zu)-3 fork wait\n", philo->idx + 1);
	if (sem_wait(info->sem_forks) != SUCCESS)
		return (PROCESS_ERROR);
	if (check_philo_died(philo, now_time) == PHILO_DIED)
	{
		print_msg(TYPE_DIED, philo);
		sem_post(info->sem_waiter);
//		printf("  (%zu)-4 died\n", philo->idx + 1);
		return (PHILO_DIED);
	}
	print_msg(TYPE_FORK, philo);

//	printf("  (%zu)-5 waiter post\n", philo->idx + 1);
	if (sem_post(info->sem_waiter) != SUCCESS)
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
	if (info->must_eat_times > 0 && (ssize_t)philo->eat_cnt >= info->must_eat_times)
		philo->is_satisfied = true;
	if (sem_post(philo->sem_philo) != SUCCESS)
		return (PROCESS_ERROR);

	return (SUCCESS);
}
