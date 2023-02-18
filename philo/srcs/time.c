/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/18 09:57:25 by takira            #+#    #+#             */
/*   Updated: 2023/02/18 09:57:25 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <sys/time.h>

time_t	get_unix_time_ms(void)
{
	struct timeval	tv;
	time_t			ret_time;

	gettimeofday(&tv, NULL);
	ret_time = tv.tv_sec * 1000
			   + tv.tv_usec / 1000;
	return (ret_time);
}

void	print_timestamp(void)
{
	struct timeval	tv;
	time_t			unix_time;
	time_t			msec;

	gettimeofday(&tv, NULL);
	unix_time = tv.tv_sec;
	msec = tv.tv_usec/1000;
	printf("%ld:%03d(ms)",unix_time, (int)msec);
}
