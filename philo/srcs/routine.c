/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/17 13:12:09 by takira            #+#    #+#             */
/*   Updated: 2023/02/21 17:05:48 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	print_eat_times(t_params *params)
{
	size_t idx;

	idx = 0;
	printf("eat times:");
	while (idx < params->num_of_philos)
	{
		printf("%zu", params->philo_info[idx].eat_times);
		idx++;
		if (idx < params->num_of_philos)
			printf(", ");
		else
			printf("\n");
	}
}

char	*get_state_str(int state)
{
	if (state == STATE_THINKING)
		return("thinking");
	if (state == STATE_HUNGRY)
		return("\x1b[33mHUNGRY\x1b[0m  ");
	if (state == STATE_WAITING)
		return("\x1b[33mWAITING\x1b[0m  ");
	if (state == STATE_EATING)
		return("eating  ");
	if (state == STATE_SLEEPING)
		return("sleeping");
	return("\x1b[31mDIED\x1b[0m    ");
}

int	is_died(t_philo_info *philo_info)
{

}

void	eating(t_philo_info *philo_info)
{

}

void	sleeping(t_philo_info *philo_info)
{

}

void	thinking(t_philo_info *philo_info)
{

}

void	*routine(void *v_philo_info)
{
	t_philo_info	*philo_info;

	philo_info = (t_philo_info *)v_philo_info;
	while (true)
	{
		take_forks(philo_info);
		eating(philo_info);
		put_forks(philo_info);
		sleeping(philo_info);
		thinking(philo_info);
		if (is_died(philo_info))
			break ;
	}
	return (NULL);
}

void	*monitor(void *v_params)
{

}
