/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_console.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/18 09:58:51 by takira            #+#    #+#             */
/*   Updated: 2023/02/27 11:57:04 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

static char	*get_print_msg(t_print_type type)
{
	if (type == TYPE_FORK)
		return (PRINT_FORK);
	if (type == TYPE_EATING)
		return (PRINT_EATING);
	if (type == TYPE_SLEEPING)
		return (PRINT_SLEEPING);
	if (type == TYPE_THINKING)
		return (PRINT_THINKING);
	if (type == TYPE_SIM_START)
		return (PRINT_SIM_START);
	return (PRINT_DIED);
}

int	print_msg_consider_died(t_philo_info *philo, t_print_type type)
{
	time_t	now_time;
	int		ret_value;

	now_time = get_unix_time_ms();
	if (check_philo_died(philo, now_time) == PHILO_DIED)
	{
		print_msg(TYPE_DIED, philo);
		return (PHILO_DIED);
	}
	ret_value = print_msg(type, philo);
	return (ret_value);
}

int	print_msg(t_print_type type, t_philo_info *philo)
{
	const size_t	idx = philo->idx;
	time_t			time;
	bool			is_died;

	if (type != TYPE_DIED)
	{
		is_died = get_is_died(philo, NULL);
		if (is_died)
			return (PHILO_DIED);
	}
	sem_wait(philo->info_ptr->sem_print);
	time = get_unix_time_ms();
	if (type == TYPE_SIM_START)
		printf("%ld:%03ld %s\n", time / 1000, time % 1000, get_print_msg(type));
	else
		printf("\x1b[48;5;%03zum%ld:%03ld %zu %s\x1b[0m\n", \
		idx % 255, time / 1000, time % 1000, idx + 1, get_print_msg(type));
	if (type == TYPE_DIED)
		return (SUCCESS);
	sem_post(philo->info_ptr->sem_print);
	return (SUCCESS);
}

//	printf("%ld%03ld %zu %s\n", unix_time_sec, unix_time_msec, idx, msg);
//printf("%ld %03d\n", tv.tv_sec, tv.tv_usec / 1000);
