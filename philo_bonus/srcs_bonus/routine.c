/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/25 16:15:23 by takira            #+#    #+#             */
/*   Updated: 2023/02/26 11:55:38 by takira           ###   ########.fr       */
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

bool	get_is_died(t_philo_info *philo)
{
	bool	is_died;

	if (sem_wait(philo->sem_philo) != SUCCESS)
		return (PROCESS_ERROR);
	is_died = philo->is_died;
	if (sem_wait(philo->sem_philo) != SUCCESS)
		return (PROCESS_ERROR);
	return (is_died);
}

size_t	get_eat_cnt(t_philo_info *philo)
{
	size_t	eat_cnt;

	if (sem_wait(philo->sem_philo) != SUCCESS)
		return (PROCESS_ERROR);
	eat_cnt = philo->eat_cnt;
	if (sem_wait(philo->sem_philo) != SUCCESS)
		return (PROCESS_ERROR);
	return (eat_cnt);
}

static int	check_philo_died(t_philo_info *philo, time_t now_time)
{
	int		ret_value;
	time_t	start_time;
	time_t	delta_time;
//	bool	is_satisfied;

	ret_value = PHILO_ALIVE;
	if (sem_wait(philo->sem_philo) != SUCCESS)
		return (PROCESS_ERROR);
//	is_satisfied = philo->is_satisfied;
	start_time = philo->start_time;

	delta_time = get_delta_time_ms(now_time, start_time);
	if (delta_time >= philo->info_ptr->time_to_die)
	{
		philo->is_died = PHILO_DIED;
		ret_value = PHILO_DIED;
	}
	if (sem_post(philo->sem_philo) != SUCCESS)
		return (PROCESS_ERROR);

	return (ret_value);
}

int	take_forks(t_philo_info *philo)
{
	t_info	*info;
	time_t	now_time;

	info = philo->info_ptr;
	if (sem_wait(info->sem_waiter) != SUCCESS)
		return (PROCESS_ERROR);

	if (sem_wait(info->sem_forks) != SUCCESS)
		return (PROCESS_ERROR);

	now_time = get_unix_time_ms();
	if (check_philo_died(philo, now_time) == PHILO_DIED)
	{
		print_msg(TYPE_DIED, philo);
		sem_post(info->sem_waiter);
		return (PHILO_DIED);
	}
	print_msg(TYPE_FORK, philo);
	if (sem_wait(info->sem_forks) != SUCCESS)
		return (PROCESS_ERROR);

	if (check_philo_died(philo, now_time) == PHILO_DIED)
	{
		print_msg(TYPE_DIED, philo);
		sem_post(info->sem_waiter);
		return (PHILO_DIED);
	}
	print_msg(TYPE_FORK, philo);

	if (sem_post(info->sem_waiter) != SUCCESS)
		return (PROCESS_ERROR);
	return (SUCCESS);
}

int	eating(t_philo_info *philo)
{
	t_info	*info;
	time_t	now_time;

	info = philo->info_ptr;

	now_time = get_unix_time_ms();
	if (check_philo_died(philo, now_time) == PHILO_DIED)
	{
		print_msg(TYPE_DIED, philo);
		return (PHILO_DIED);
	}
	print_msg(TYPE_EATING, philo);
	if (sem_wait(philo->sem_philo) != SUCCESS)
		return (PROCESS_ERROR);
	philo->start_time = get_unix_time_ms();
	if (sem_post(philo->sem_philo) != SUCCESS)
		return (PROCESS_ERROR);

	sleep_ms(info->time_to_eat);
	return (SUCCESS);
}

int	put_forks_and_update_eat_times(t_philo_info *philo)
{
	t_info	*info;

	info = philo->info_ptr;
	if (sem_post(info->sem_forks) != SUCCESS)
		return (PROCESS_ERROR);
	if (sem_post(info->sem_forks) != SUCCESS)
		return (PROCESS_ERROR);

	if (sem_wait(philo->sem_philo) != SUCCESS)
		return (PROCESS_ERROR);
	philo->eat_cnt++;
	if (philo->eat_cnt >= info->must_eat_times)
		philo->is_satisfied = true;
	if (sem_post(philo->sem_philo) != SUCCESS)
		return (PROCESS_ERROR);

	return (SUCCESS);
}

int	sleeping_and_thinking(t_philo_info *philo)
{
	t_info	*info;
	time_t	now_time;

	info = philo->info_ptr;

	// sleeping
	now_time = get_unix_time_ms();
	if (check_philo_died(philo, now_time) == PHILO_DIED)
	{
		print_msg(TYPE_DIED, philo);
		return (PHILO_DIED);
	}
	print_msg(TYPE_SLEEPING, philo);
	sleep_ms(info->time_to_sleep);

	// thinking
	now_time = get_unix_time_ms();
	if (check_philo_died(philo, now_time) == PHILO_DIED)
	{
		print_msg(TYPE_DIED, philo);
		return (PHILO_DIED);
	}
	print_msg(TYPE_THINKING, philo);
	return (SUCCESS);
}

int	check_continue(t_philo_info *philo)
{
	bool	is_satisfied;
	bool	is_died;

	if (sem_wait(philo->sem_philo) != SUCCESS)
		return (PROCESS_ERROR);
	is_died = philo->is_died;
	is_satisfied = philo->is_satisfied;
	if (sem_wait(philo->sem_philo) != SUCCESS)
		return (PROCESS_ERROR);
	if (is_died)
		return (PHILO_DIED);
	if (is_satisfied)
		return (BREAK);
	return (CONTINUE);
}

void	*routine(t_philo_info *philo)
{
	int	ret_value;

	ret_value = CONTINUE;
	while (true)
	{
		ret_value = take_forks(philo);
		if (ret_value != SUCCESS)
			break ;

		ret_value = eating(philo);
		if (ret_value != SUCCESS)
			break ;

		ret_value = put_forks_and_update_eat_times(philo);
		if (ret_value != SUCCESS)
			break ;

		ret_value = sleeping_and_thinking(philo);
		if (ret_value != SUCCESS)
			break ;

		ret_value = check_continue(philo);
		if (ret_value != CONTINUE)
			break ;
	}
	if (ret_value == PHILO_DIED)
		return ((void *)PHILO_DIED);
	if (ret_value == PROCESS_ERROR)
		return ((void *)PROCESS_ERROR);
	return ((void *)SUCCESS);
}

void	*monitor(t_philo_info *philo)
{
	int		ret_value;
	time_t	now_time;

	ret_value = CONTINUE;
	while (ret_value == SUCCESS || ret_value == CONTINUE)
	{
		usleep(500);
		now_time = get_unix_time_ms();
		ret_value = check_philo_died(philo, now_time);
	}
	if (ret_value == PHILO_DIED)
		return ((void *)PHILO_DIED);
	if (ret_value == PROCESS_ERROR)
		return ((void *)PROCESS_ERROR);
	return ((void *)SUCCESS);
}






















