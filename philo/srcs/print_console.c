/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_console.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/18 09:58:51 by takira            #+#    #+#             */
/*   Updated: 2023/02/23 15:33:53 by takira           ###   ########.fr       */
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
	if (type == TYPE_SIM_START)
		return (PRINT_SIM_START);
	return (PRINT_DIED);
}

int	print_msg(size_t idx, t_print_type type, t_params *params)
{
	struct timeval	tv;
	int				is_died;

	if (pthread_mutex_lock(&params->print_mutex) != SUCCESS)
		return (PROCESS_ERROR);
	is_died = PHILO_ALIVE;
	if (type != TYPE_DIED)
	{
		pthread_mutex_lock(&params->died_mutex);
		is_died = params->is_died;
		pthread_mutex_unlock(&params->died_mutex);
	}
	if (type != TYPE_DIED && is_died == PHILO_DIED)
	{
		pthread_mutex_unlock(&params->print_mutex);
		return (PHILO_DIED);
	}
	gettimeofday(&tv, NULL);
	if (type == TYPE_SIM_START)
		printf("%ld%03d %s\n", tv.tv_sec, tv.tv_usec / 1000, get_print_msg(type));
	else
		printf("\x1b[48;5;%03zum%ld%03d %zu %s\x1b[0m\n", \
		idx % 255, tv.tv_sec, tv.tv_usec / 1000, idx + 1, get_print_msg(type));
	if (pthread_mutex_unlock(&params->print_mutex) != SUCCESS)
		return (PROCESS_ERROR);
	return (SUCCESS);
}

//	printf("%ld%03ld %zu %s\n", unix_time_sec, unix_time_msec, idx, msg);
