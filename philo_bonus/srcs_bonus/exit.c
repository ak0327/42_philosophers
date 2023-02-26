/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/30 19:40:19 by takira            #+#    #+#             */
/*   Updated: 2023/02/26 19:55:43 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

int	print_err_msg_and_free(int err, t_info *info, int ret)
{
	if (err == INVALID_ARG_COUNT)
		printf("[Error] Invalid argument. Input as following:\n" \
		"$>./philo NumOfPhilos[1,200] TimeToDie[60,LLMAX] " \
		"TimeToEat[60,LLMAX] TimeToSleep[60,LLMAX] (MustEatTimes[1,LLMAX])\n");
	else if (err == INVALID_NUM_OF_PHILOS)
		printf("[Error] Invalid argument. 1 <= NumOfPhilos <= 200\n");
	else if (err == INVALID_TIME_TO_DIE)
		printf("[Error] Invalid argument. 60 <= TimeToDie\n");
	else if (err == INVALID_TIME_TO_EAT)
		printf("[Error] Invalid argument. 60 <= TimeToEat\n");
	else if (err == INVALID_TIME_TO_SLEEP)
		printf("[Error] Invalid argument. 60 <= TimeToSleep\n");
	else if (err == INVALID_MUST_EAT_TIMES)
		printf("[Error] Invalid argument. 1 <= MustEatTimes\n");
	else
		printf("[Error] Process abort\n");
	free_info(&info);
	return (ret);
}
