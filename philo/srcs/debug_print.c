/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug_print.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/22 10:56:56 by takira            #+#    #+#             */
/*   Updated: 2023/02/24 14:20:44 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	print_eat_times(t_params *params)
{
	size_t	idx;

	idx = 0;
	printf("start_eating times:");
	while (idx < params->num_of_philos)
	{
		printf("%zu", params->philo_info[idx].eat_times);
		idx++;
		if (idx < params->num_of_philos)
			printf(", ");
		else
			printf("\n");
	}
}

void	print_timestamp(void)
{
	t_timeval	tv;
	time_t		unix_time;
	time_t		msec;

	gettimeofday(&tv, NULL);
	unix_time = tv.tv_sec;
	msec = tv.tv_usec / 1000;
	printf("%ld:%03d(ms)", unix_time, (int)msec);
}
