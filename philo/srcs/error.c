/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/30 19:40:19 by takira            #+#    #+#             */
/*   Updated: 2023/02/17 10:28:40 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	print_error(int err)
{
	if (err == INVALID_ARG)
		printf("[Error] Invalid argument. Input as following:\n" \
		"$>./args NumOfPhilos[1,200] TimeToDie[60,INT_MAX] " \
		"TimeToEat[60,INT_MAX] TimeToSleep[60,INT_MAX] (MustEatTimes[0,INT_MAX])\n");
	else if (err == PROCESS_ERROR)
		printf("[Error] Process error occurred\n");
}