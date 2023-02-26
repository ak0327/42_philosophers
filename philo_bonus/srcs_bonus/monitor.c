/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/26 16:08:07 by takira            #+#    #+#             */
/*   Updated: 2023/02/26 22:39:30 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	*monitor(t_philo_info *philo)
{
	int		ret_value;
	time_t	now_time;

	ret_value = CONTINUE;
	while (true)
	{
		usleep(100);
		now_time = get_unix_time_ms();
		ret_value = check_philo_died(philo, now_time);
		if (ret_value == PHILO_DIED)
			break ;
		ret_value = check_continue(philo);
		if (ret_value != CONTINUE)
			break ;
	}
//	printf("(%zu)monitor fin\n", philo->idx+1);
	if (ret_value == PHILO_DIED)
		return ((void *)PHILO_DIED);
	if (ret_value == PROCESS_ERROR)
		return ((void *)PROCESS_ERROR);
	return ((void *)SUCCESS);
}
