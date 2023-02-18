/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_console.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/18 09:58:51 by takira            #+#    #+#             */
/*   Updated: 2023/02/18 15:45:18 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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
	return (PRINT_DIED);
}

void	print_msg(size_t idx, t_print_type type, time_t time, t_params *params)
{
	const time_t	unix_time_sec = time / 1000;
	const time_t	unix_time_msec = time % 1000;
	const char		*msg = get_print_msg(type);

	if (!params->is_died || params->died_philo == (ssize_t)idx)
	{
//		printf("[%zu] %ld:%03ld(ms) %03zu %s\n", idx, unix_time_sec, unix_time_msec, idx, msg);
		printf("%ld%03ld %zu %s\n", unix_time_sec, unix_time_msec, idx, msg);
	}
}
