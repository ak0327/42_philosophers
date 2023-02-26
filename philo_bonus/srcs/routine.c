/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/25 16:15:23 by takira            #+#    #+#             */
/*   Updated: 2023/02/26 10:08:39 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "philo_bonus.h"

// pthread_create
// routineでmutexでなくsemで管理するだけ？
// philoはプロセスなのでforkしなければならない

// forkはsemで抑えられる
// 各philoのeat timesやdiedの同期をどう実装する？？
// 単純に順番にとっていくだけ...?
// waiterをとって、2ほんのforkを同時に獲得しておく
// 食べて、寝る
// そうすると順番は担保される
// bonusは成り行きなのか...？

// wait中のdied検知が必要、これが問題
// どうやってparent process, child processでデータを共有する？

// main
//  |--- fork -> philo 1
//  |--- fork -> philo 2
//  .
//  |--- fork -> philo n


int	take_forks(t_philo_info *philo)
{
	t_info	*info;

	info = philo->info_ptr;
	if (sem_wait(info->sem_waiter) != SUCCESS)
		return (PROCESS_ERROR);
	if (sem_wait(info->sem_forks) != SUCCESS)
		return (PROCESS_ERROR);
	print_msg(philo->idx, TYPE_FORK, info);
	if (sem_wait(info->sem_forks) != SUCCESS)
		return (PROCESS_ERROR);
	print_msg(philo->idx, TYPE_FORK, info);
	if (sem_post(info->sem_waiter) != SUCCESS)
		return (PROCESS_ERROR);
	return (SUCCESS);
}

int	put_forks(t_philo_info *philo)
{
	t_info	*info;

	info = philo->info_ptr;
	if (sem_post(info->sem_forks) != SUCCESS)
		return (PROCESS_ERROR);
	if (sem_post(info->sem_forks) != SUCCESS)
		return (PROCESS_ERROR);
	philo->eat_cnt++;
	if (philo->eat_cnt >= info->must_eat_times)
		philo->is_satisfied = true;
	return (SUCCESS);
}

int	eating(t_philo_info *philo)
{
	t_info	*info;

	info = philo->info_ptr;

	print_msg(philo->idx, TYPE_SLEEPING, info);
	philo->start_time = get_unix_time_ms();
	sleep_ms(info->time_to_eat);
	return (SUCCESS);
}

int	sleeping_and_thinking(t_philo_info *philo)
{
	t_info	*info;

	info = philo->info_ptr;

	// sleeping
	print_msg(philo->idx, TYPE_SLEEPING, info);
	sleep_ms(info->time_to_sleep);

	// thinking
	print_msg(philo->idx, TYPE_THINKING, info);
	return (SUCCESS);
}

void	*routine(t_philo_info *philo)
{
	while (true)
	{
		if (take_forks(philo) == PROCESS_ERROR)
			return ((void *)PROCESS_ERROR);
		eating(philo);
		if (put_forks(philo) == PROCESS_ERROR)
			return ((void *)PROCESS_ERROR);
		sleeping_and_thinking(philo);
		if (philo->is_satisfied)
			break ;
	}
	return ((void *)SUCCESS);
}

void	*monitor(t_philo_info *philo)
{
	while (true)
	{

	}
}

// died -> kill ?

// 各philo内でdied, satisfiedをチェック