/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fork_wait.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/26 19:22:16 by takira            #+#    #+#             */
/*   Updated: 2023/02/26 22:39:01 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

static int	run_pthread(t_philo_info *philo)
{
	void	*status;
	int		ret_value;

//	printf("[#run](%zu)-1\n", philo->idx);
	if (pthread_create(\
	&philo->philo_tid, NULL, (void *)routine, (void *)philo) != SUCCESS)
		return (PROCESS_ERROR);
//	printf("[#run](%zu)-2\n", philo->idx);
	if (pthread_create(\
	&philo->monitor_tid, NULL, (void *)monitor, (void *)philo) != SUCCESS)
		return (PROCESS_ERROR);
//	printf("[#run](%zu)-3\n", philo->idx);
//	printf("[#run](%zu)-4\n", philo->idx);
	if (pthread_join(philo->monitor_tid, &status) != SUCCESS)
		return (PROCESS_ERROR);
	if (pthread_detach(philo->philo_tid) != SUCCESS)
		return (PROCESS_ERROR);
	ret_value = (int)(long)status;
//	printf("[#run](%zu)-5, s1:%ld, s2:%ld\n", philo->idx, (long)s1, (long)s2);
	return (ret_value);
}

static int	wait_and_terminate_process(t_info *info)
{
	int				ret_value;
	size_t			idx;

	idx = 0;
	while (idx < info->num_of_philos)
	{
		waitpid(-1, &ret_value, 0);
		ret_value = WEXITSTATUS(ret_value);
		if (ret_value == PHILO_DIED)
		{
			kill(0, SIGINT);
			return (SUCCESS);
		}
		else if (ret_value == PROCESS_ERROR)
		{
			kill(0, SIGINT);
			return (PROCESS_ERROR);
		}
		idx++;
	}
	return (ret_value);
}

int	start_routine(t_info *info)
{
	time_t	now_time;
	size_t	idx;
	int		ret_value;

	idx = 0;
	now_time = get_unix_time_ms();
	print_msg(TYPE_SIM_START, &info->philo_info[idx]);
//	printf("%ld%03ld %s\n", now_time / 1000, now_time % 1000, PRINT_SIM_START);
	while (idx < info->num_of_philos)
	{
		info->philo_info[idx].start_time = now_time;
		info->philo_info[idx].pid = fork();
		if (info->philo_info[idx].pid < 0)
			return (PROCESS_ERROR);
		if (info->philo_info[idx].pid == CHILD_PROCESS)
			exit (run_pthread(&info->philo_info[idx]));
		idx++;
	}
	ret_value = wait_and_terminate_process(info);
	return (ret_value);
}
