/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/30 19:40:19 by takira            #+#    #+#             */
/*   Updated: 2023/02/19 13:14:25 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	print_err_msg_and_free_allocs(int err, t_params *params, int ret)
{
	if (err == INVALID_ARG_COUNT)
		printf("[Error] Invalid argument. Input as following:\n" \
		"$>./philo NumOfPhilos[1,200] TimeToDie[60,LLMAX] " \
		"TimeToEat[60,LLMAX] TimeToSleep[60,LLMAX] (MustEatTimes[0,LLMAX])\n");
	else if (err == INVALID_NUM_OF_PHILOS)
		printf("[Error] Invalid argument. 1 <= NumOfPhilos <= 200\n");
	else if (err == INVALID_TIME_TO_DIE)
		printf("[Error] Invalid argument. 60 <= TimeToDie\n");
	else if (err == INVALID_TIME_TO_EAT)
		printf("[Error] Invalid argument. 60 <= TimeToEat\n");
	else if (err == INVALID_TIME_TO_SLEEP)
		printf("[Error] Invalid argument. 60 <= TimeToSleep\n");
	else if (err == INVALID_MUST_EAT_TIMES)
		printf("[Error] Invalid argument. 0 <= MustEatTimes\n");
	else
		printf("[Error] Process error occurred\n");
	free_allocs(params);
	return (ret);
}

void	free_allocs(t_params *params)
{
	size_t	idx;

	if (!params)
		return ;
	free(params->philo_no);
	free(params->forks);
	free(params->eat_times);
	free(params->lock_each_fork);
	idx = 0;
	while (params->philo_info && idx < params->num_of_philos)
	{
		free(params->philo_info[idx].wait_info);
		idx++;
	}
	free(params->philo_info);
	free(params->state);
	free(params);
}
