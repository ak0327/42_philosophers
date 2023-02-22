/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/30 19:40:19 by takira            #+#    #+#             */
/*   Updated: 2023/02/22 18:58:02 by takira           ###   ########.fr       */
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
		printf("[Error] Process abort\n");
	free_params(&params);
	return (ret);
}

static void	free_ret_nullptr(void **ptr)
{
	if (!ptr)
		return ;
	free(*ptr);
	*ptr = NULL;
}

void	free_params(t_params **params)
{
	if (!params)
		return ;
	free_ret_nullptr((void **)&(*params)->philo_tid);
	free_ret_nullptr((void **)&(*params)->philo_info);
	free_ret_nullptr((void **)&(*params)->prev_used_by);
//	free_ret_nullptr((void **)&(*params)->held_by);
//	free_ret_nullptr((void **)&(*params)->state);
	free_ret_nullptr((void **)&(*params)->fork_mutex);
	free_ret_nullptr((void **)&(*params)->prev_used_mutex);
	free_ret_nullptr((void **)params);
}
