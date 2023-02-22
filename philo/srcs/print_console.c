/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_console.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/18 09:58:51 by takira            #+#    #+#             */
/*   Updated: 2023/02/22 19:49:48 by takira           ###   ########.fr       */
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

int	print_msg(size_t idx, t_print_type type, t_params *params, time_t time)
{
	const time_t	unix_time_sec = time / 1000;
	const time_t	unix_time_msec = time % 1000;
	int				is_died;
	ssize_t			died_idx;

	if (pthread_mutex_lock(&params->print_mutex) != SUCCESS)
		return (PROCESS_ERROR);
	if (type != TYPE_DIED)
	{
		is_died = get_is_died(params, &died_idx, SUCCESS);
		if (is_died == PHILO_DIED)
		{
			if (pthread_mutex_unlock(&params->print_mutex) != SUCCESS)
				return (PROCESS_ERROR);
			return (PHILO_DIED);
		}
	}
	printf("\x1b[48;5;%03zum%ld%03ld %zu %s\x1b[0m\n", \
	idx % 255, unix_time_sec, unix_time_msec, idx + 1, get_print_msg(type));
	if (pthread_mutex_unlock(&params->print_mutex) != SUCCESS)
		return (PROCESS_ERROR);
	return (SUCCESS);
}

//	printf("%ld%03ld %zu %s\n", unix_time_sec, unix_time_msec, idx, msg);
