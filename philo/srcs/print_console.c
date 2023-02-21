/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_console.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/18 09:58:51 by takira            #+#    #+#             */
/*   Updated: 2023/02/21 22:47:00 by takira           ###   ########.fr       */
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

//TODO: idx->+1
void	print_msg(size_t idx, t_print_type type, t_params *params)
{
	const time_t	time = get_unix_time_ms();
	const time_t	unix_time_sec = time / 1000;
	const time_t	unix_time_msec = time % 1000;
	const char		*msg = get_print_msg(type);

	if (params->is_died)
		if (!(params->died_idx == (ssize_t)idx && type == TYPE_DIED))
			return ;

	pthread_mutex_lock(&params->print_mutex);
//	printf("\x1b[48;5;%03zum%ld%03ld %zu %s\x1b[0m\n", idx % 255, unix_time_sec, unix_time_msec, idx, msg);
	printf("%ld%03ld %zu %s\n", unix_time_sec, unix_time_msec, idx, msg);

	//// debug ////
//	printf("\x1b[48;5;%03zum[%zu]%ld:%03ld %zu %s\x1b[0m\n",idx, idx % 255, unix_time_sec, unix_time_msec, idx, msg);
	///////////////
	pthread_mutex_unlock(&params->print_mutex);
	if (params->is_died && params->died_idx == (ssize_t)idx)
		printf("dt:%ld\n", time - params->philo_info[idx].start_time);
}
