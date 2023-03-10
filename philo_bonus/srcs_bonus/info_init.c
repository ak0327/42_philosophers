/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_params.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/17 10:00:01 by takira            #+#    #+#             */
/*   Updated: 2023/02/27 10:55:43 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

static char	*get_sem_name(size_t idx)
{
	char	*sem_name;
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
	free(itoa);
	return (sem_name);
}

static int	init_alloc(t_info **info)
{
	size_t	idx;

	(*info)->philo_info = \
	(t_philo_info *)malloc(sizeof(t_philo_info) * (*info)->num_of_philos);
	if (!(*info)->philo_info)
		return (FAILURE);
	idx = 0;
	while (idx < (*info)->num_of_philos)
	{
		(*info)->philo_info[idx].idx = idx;
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

static int	init_sem_philos(t_info *info)
{
	size_t	idx;
	char	*sem_name;

	idx = 0;
	while (idx < info->num_of_philos)
	{
		sem_name = info->philo_info[idx].sem_name;
		sem_unlink(sem_name);
		info->philo_info[idx].sem_philo = sem_open(sem_name, O_CREAT, 0777, 1);
		if (info->philo_info[idx].sem_philo == SEM_FAILED)
			return (FAILURE);
		idx++;
	}
	return (SUCCESS);
}

static int	init_semaphore(t_info *info)
{
	sem_unlink(SEM_FORKS);
	info->sem_forks = sem_open(SEM_FORKS, O_CREAT, 0777, info->num_of_philos);
	if (info->sem_forks == SEM_FAILED)
		return (FAILURE);
	sem_unlink(SEM_WAITER);
	info->sem_waiter = sem_open(SEM_WAITER, O_CREAT, 0777, 1);
	if (info->sem_waiter == SEM_FAILED)
		return (FAILURE);
	sem_unlink(SEM_PRINT);
	info->sem_print = sem_open(SEM_PRINT, O_CREAT, 0777, 1);
	if (info->sem_print == SEM_FAILED)
		return (FAILURE);
	if (init_sem_philos(info) == FAILURE)
		return (FAILURE);
	return (SUCCESS);
}

int	init_info(int argc, char **argv, t_info **info)
{
	int	is_process_success;
	int	ret_value;

	if (!(5 <= argc && argc <= 6))
		return (INVALID_ARG_COUNT);
	*info = (t_info *)ft_calloc(sizeof(t_info), 1);
	if (!*info)
		return (PROCESS_ERROR);
	memset(*info, 0, sizeof(t_info));
	ret_value = get_input_args(argv, *info);
	if (ret_value != SUCCESS)
		return (ret_value);
	(*info)->is_died = PHILO_ALIVE;
	(*info)->start_time = 0;
	is_process_success = SUCCESS;
	is_process_success += init_alloc(info);
	is_process_success += init_semaphore(*info);
	if (is_process_success == SUCCESS)
		return (SUCCESS);
	free_info(info);
	return (FAILURE);
}
