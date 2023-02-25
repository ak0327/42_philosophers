/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/30 19:40:19 by takira            #+#    #+#             */
/*   Updated: 2023/02/25 10:56:54 by takira           ###   ########.fr       */
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

void	print_msg_when_terminate_philo(int ret_value, t_philo_info *philo)
{
	ssize_t	died_philo;

	if (ret_value == PROCESS_ERROR)
		printf("[Error] Process abort\n");
	get_is_died(philo->info_ptr, &died_philo, SUCCESS);
	if (ret_value == PHILO_DIED && died_philo == (ssize_t)philo->idx)
		print_msg(philo->idx, TYPE_DIED, philo->info_ptr);
}
