/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_console.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/18 09:58:51 by takira            #+#    #+#             */
/*   Updated: 2023/02/24 14:23:37 by takira           ###   ########.fr       */
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

time_t	get_print_time(t_philo_info *philo, t_print_type type)
{
	time_t	now_time;
	time_t	start_time;
	time_t	time_to_die;

	now_time = get_unix_time_ms();
	if (type != TYPE_DIED)
		return (now_time);
	start_time = get_start_time(philo);
	time_to_die = philo->params_ptr->time_to_die;
	now_time = start_time + time_to_die;
	return (now_time);
}

int	print_msg(size_t idx, t_print_type type, t_params *params)
{
	time_t	time;
	int		is_died;
	ssize_t	died_philo;

	if (pthread_mutex_lock(&params->print_mutex) != SUCCESS)
		return (PROCESS_ERROR);
	is_died = get_is_died(params, &died_philo, SUCCESS);
	if (is_died == PHILO_DIED && type != TYPE_DIED)
	{
		pthread_mutex_unlock(&params->print_mutex);
		return (PHILO_DIED);
	}
	time = get_print_time(&params->philo_info[idx], type);
	if (type == TYPE_SIM_START)
		printf("%ld%03ld %s\n", time / 1000, time % 1000, get_print_msg(type));
	else
		printf("\x1b[48;5;%03zum%ld%03ld %zu %s\x1b[0m\n", \
		idx % 255, time / 1000, time % 1000, idx + 1, get_print_msg(type));
	if (pthread_mutex_unlock(&params->print_mutex) != SUCCESS)
		return (PROCESS_ERROR);
	return (SUCCESS);
}

//	printf("%ld%03ld %zu %s\n", unix_time_sec, unix_time_msec, idx, msg);
//printf("%ld %03d\n", tv.tv_sec, tv.tv_usec / 1000);
