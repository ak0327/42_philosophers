/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   params_destroy.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/22 23:24:43 by takira            #+#    #+#             */
/*   Updated: 2023/02/26 19:24:54 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

static void	free_ret_nullptr(void **ptr)
{
	if (!ptr)
		return ;
	free(*ptr);
	*ptr = NULL;
}

void	free_info(t_info **info)
{
	size_t	idx;

	if (!info || !*info)
		return ;
	idx = 0;
	while (idx < (*info)->num_of_philos)
	{
		free((*info)->philo_info[idx].sem_name);
		idx++;
	}
	free_ret_nullptr((void **)&(*info)->philo_info);
	free_ret_nullptr((void **)info);
}

int	destroy_info(t_info *info)
{
	size_t	idx;

	sem_close(info->sem_forks);
	sem_unlink(SEM_FORKS);
	sem_close(info->sem_waiter);
	sem_unlink(SEM_WAITER);
	sem_close(info->sem_print);
	sem_unlink(SEM_PRINT);
	idx = 0;
	while (idx < info->num_of_philos)
	{
		sem_close(info->philo_info[idx].sem_philo);
		sem_unlink(info->philo_info[idx].sem_name);
		idx++;
	}
	free_info(&info);
	return (SUCCESS);
}
