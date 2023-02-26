/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/25 16:15:23 by takira            #+#    #+#             */
/*   Updated: 2023/02/26 16:12:59 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "philo_bonus.h"

void	wait_while_philo_alive(t_philo_info *philo)
{
	while (true)
	{
		if (get_is_died(philo) == true)
			break ;
		usleep(500);
	}
}

int	eating(t_philo_info *philo)
{
	t_info	*info;
	time_t	now_time;

	info = philo->info_ptr;

	now_time = get_unix_time_ms();
	if (check_philo_died(philo, now_time) == PHILO_DIED)
	{
		print_msg(TYPE_DIED, philo);
		return (PHILO_DIED);
	}
	print_msg(TYPE_EATING, philo);

	if (info->num_of_philos == 1)
	{
		wait_while_philo_alive(philo);
		print_msg(TYPE_DIED, philo);
		return (PHILO_DIED);
	}
	if (sem_wait(philo->sem_philo) != SUCCESS)
		return (PROCESS_ERROR);
	philo->start_time = get_unix_time_ms();
	if (sem_post(philo->sem_philo) != SUCCESS)
		return (PROCESS_ERROR);

	sleep_ms(info->time_to_eat);
	return (SUCCESS);
}

int	sleeping_and_thinking(t_philo_info *philo)
{
	t_info	*info;
	time_t	now_time;

	info = philo->info_ptr;

	// sleeping
	now_time = get_unix_time_ms();
	if (check_philo_died(philo, now_time) == PHILO_DIED)
	{
		print_msg(TYPE_DIED, philo);
		return (PHILO_DIED);
	}
	print_msg(TYPE_SLEEPING, philo);
	sleep_ms(info->time_to_sleep);

	// thinking
	now_time = get_unix_time_ms();
	if (check_philo_died(philo, now_time) == PHILO_DIED)
	{
		print_msg(TYPE_DIED, philo);
		return (PHILO_DIED);
	}
	print_msg(TYPE_THINKING, philo);
	return (SUCCESS);
}

void	*routine(t_philo_info *philo)
{
	int	ret_value;

	ret_value = CONTINUE;
//	printf(" (%zu)routine start\n", philo->idx + 1);
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
	printf(" (%zu)routine end, ret:%d, eat:%zu\n", philo->idx + 1, ret_value, philo->eat_cnt);
	if (ret_value == PHILO_DIED)
		return ((void *)PHILO_DIED);
	if (ret_value == PROCESS_ERROR)
		return ((void *)PROCESS_ERROR);
	return ((void *)SUCCESS);
}
