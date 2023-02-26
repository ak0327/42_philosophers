/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/18 09:57:25 by takira            #+#    #+#             */
/*   Updated: 2023/02/24 15:53:57 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

time_t	get_unix_time_ms(void)
{
	t_timeval	tv;
	time_t		ret_time;

	gettimeofday(&tv, NULL);
	ret_time = tv.tv_sec * 1000 + tv.tv_usec / 1000;
	return (ret_time);
}

time_t	get_delta_time_ms(time_t now_ms, time_t start_ms)
{
	return (now_ms - start_ms);
}

static time_t	get_delta_timeval_ms(t_timeval now_tv, t_timeval start_tv)
{
	return ((time_t)(now_tv.tv_sec * 1000 + now_tv.tv_usec / 1000) \
	- (time_t)(start_tv.tv_sec * 1000 + start_tv.tv_usec / 1000));
}

void	sleep_ms(time_t time_ms)
{
	t_timeval	start_tv;
	t_timeval	now_tv;

	gettimeofday(&start_tv, NULL);
	while (true)
	{
		gettimeofday(&now_tv, NULL);
		if (get_delta_timeval_ms(now_tv, start_tv) >= time_ms)
			break ;
		usleep(500);
	}
}
