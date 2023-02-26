/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_params.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/17 10:00:01 by takira            #+#    #+#             */
/*   Updated: 2023/02/26 16:59:21 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

static char *get_sem_name(size_t idx)
{
	char 	*sem_name;
	char	*itoa;
	size_t	len;

	if (idx > INT_MAX - 1)
		return (NULL);
	itoa = ft_itoa((int)idx + 1);
	if (!itoa)
		return (NULL);
	len = ft_strlen_ns(SEM_PHILO) + ft_strlen_ns(itoa);
	sem_name = (char *) ft_calloc(sizeof(char), len + 1);
	if (!sem_name)
	{
		free(itoa);
		return (NULL);
	}
	ft_strlcat_ns(sem_name, SEM_PHILO, len + 1);
	ft_strlcat_ns(sem_name, itoa, len + 1);
	return (sem_name);
}

static int	init_alloc(t_info **info)
{
	size_t	idx;

	(*info)->philo_info = \
	(t_philo_info *)malloc(sizeof(t_philo_info) * (*info)->num_of_philos);
//	(*info)->philo_info = \
//	(t_philo_info *)ft_calloc(sizeof(t_philo_info), (*info)->num_of_philos);
	if (!(*info)->philo_info )
		return (FAILURE);
	idx = 0;
	while (idx < (*info)->num_of_philos)
	{
		(*info)->philo_info[idx].idx = idx;
		(*info)->philo_info[idx].status = PH_THINKING;
		(*info)->philo_info[idx].start_time = 0;
		(*info)->philo_info[idx].eat_cnt = 0;
		(*info)->philo_info[idx].info_ptr = *info;
		(*info)->philo_info[idx].pid = 0;
		(*info)->philo_info[idx].is_satisfied = false;
		(*info)->philo_info[idx].sem_name = get_sem_name(idx);
		if (!(*info)->philo_info[idx].sem_name)
			return (FAILURE);
		idx++;
	}
	return (SUCCESS);
}

static int	init_semaphore(t_info *info)
{
	size_t	idx;
	char	*sem_name;

	sem_unlink(SEM_FORKS);
	info->sem_forks = sem_open(SEM_FORKS, O_CREAT | O_EXCL, 0644, info->num_of_philos);
	if (info->sem_forks == SEM_FAILED)
		return (FAILURE);

	sem_unlink(SEM_WAITER);
	info->sem_waiter = sem_open(SEM_WAITER, O_CREAT | O_EXCL, 0644, 1);
	if (info->sem_waiter == SEM_FAILED)
		return (FAILURE);

	idx = 0;
	while (idx < info->num_of_philos)
	{
		sem_name = info->philo_info[idx].sem_name;
		sem_unlink(sem_name);
		info->philo_info[idx].sem_philo = sem_open(sem_name, O_CREAT | O_EXCL, 0644, 1);
		if (info->philo_info[idx].sem_philo == SEM_FAILED)
			return (FAILURE);
		idx++;
	}
	return (SUCCESS);
}

int	init_info(int argc, char **argv, t_info **info)
{
	int	is_process_success;

	if (!(5 <= argc && argc <= 6))
		return (INVALID_ARG_COUNT);
	*info = (t_info *)ft_calloc(sizeof(t_info), 1);
	if (!*info)
		return (PROCESS_ERROR);
	memset(*info, 0, sizeof(t_info));
	is_process_success = true;
	is_process_success |= get_input_args(argv, *info);
	is_process_success |= init_alloc(info);
	is_process_success |= init_semaphore(*info);
	(*info)->is_died = PHILO_ALIVE;
	(*info)->died_idx = -1;
	(*info)->is_sim_fin = CONTINUE;
	if (is_process_success)
		return (SUCCESS);
	free_info(info);
	return (FAILURE);
}
