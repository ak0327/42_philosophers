/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/18 09:57:25 by takira            #+#    #+#             */
/*   Updated: 2023/02/24 13:08:21 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <sys/time.h>

time_t	get_unix_time_ms(void)
{
	t_timeval	tv;
	time_t		ret_time;

	gettimeofday(&tv, NULL);
	ret_time = tv.tv_sec * 1000 + tv.tv_usec / 1000;
	return (ret_time);
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

time_t	get_start_time(t_philo_info *philo)
{
	time_t	start_time;

	pthread_mutex_lock(&philo->philo_mutex);
	start_time = philo->start_time;
	pthread_mutex_unlock(&philo->philo_mutex);
	return (start_time);
}

time_t	get_delta_time(time_t now_ms, time_t start_ms)
{
	return (now_ms - start_ms);
}
