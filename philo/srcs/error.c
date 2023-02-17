/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/30 19:40:19 by takira            #+#    #+#             */
/*   Updated: 2023/02/17 11:36:20 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	print_error(int err)
{
	if (err == INVALID_ARGC)
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
}