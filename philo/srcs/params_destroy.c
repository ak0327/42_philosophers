/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   params_destroy.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/22 23:24:43 by takira            #+#    #+#             */
/*   Updated: 2023/02/26 13:49:02 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	free_ret_nullptr(void **ptr)
{
	if (!ptr)
		return ;
	free(*ptr);
	*ptr = NULL;
}

void	free_params(t_params **params)
{
	if (!params || !*params)
		return ;
	free_ret_nullptr((void **)&(*params)->philo_tid);
	free_ret_nullptr((void **)&(*params)->philo_info);
	free_ret_nullptr((void **)&(*params)->prev_used_by);
	free_ret_nullptr((void **)&(*params)->fork_mutex);
	free_ret_nullptr((void **)&(*params)->prev_used_mutex);
	free_ret_nullptr((void **)params);
}

int	destroy_params(t_params *params)
{
	size_t	idx;

	idx = 0;
	while (idx < params->num_of_philos)
	{
		if (pthread_mutex_destroy(&params->fork_mutex[idx]) != SUCCESS)
			return (PROCESS_ERROR);
		if (pthread_mutex_destroy(&params->prev_used_mutex[idx]) != SUCCESS)
			return (PROCESS_ERROR);
		if (pthread_mutex_destroy(\
		&params->philo_info[idx].philo_mutex) != SUCCESS)
			return (PROCESS_ERROR);
		idx++;
	}
	if (pthread_mutex_destroy(&params->print_mutex) != SUCCESS)
		return (PROCESS_ERROR);
	if (pthread_mutex_destroy(&params->died_mutex) != SUCCESS)
		return (PROCESS_ERROR);
	free_params(&params);
	return (SUCCESS);
}
