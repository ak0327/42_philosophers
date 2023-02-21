/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_forks.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/20 09:56:44 by takira            #+#    #+#             */
/*   Updated: 2023/02/21 13:07:35 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

bool	is_philo_can_eating_wo_lock_state(t_params *params, size_t idx)
{
	const size_t	left_idx = params->philo_info[idx].left_philo_idx;
	const size_t	right_idx = params->philo_info[idx].right_philo_idx;
//	size_t			left_eat_times;
//	size_t			right_eat_times;

	return ((params->state[idx] == STATE_WAITING \
	|| params->state[idx] == STATE_THINKING) \
	&& params->state[left_idx] != STATE_EATING \
	&& params->state[right_idx] != STATE_EATING);

//	return ((params->state[idx] != STATE_EATING \
//	&& params->state[left_idx] != STATE_EATING \
//	&& params->state[right_idx] != STATE_EATING));
	if ((params->state[idx] == STATE_WAITING \
	|| params->state[idx] == STATE_THINKING) \
	&& params->state[left_idx] != STATE_EATING \
	&& params->state[right_idx] != STATE_EATING)
		return (true);

	if (params->state[idx] == STATE_DIED \
	|| params->state[left_idx] == STATE_DIED \
	|| params->state[right_idx] == STATE_DIED)
		return (false);
	return (false);
}

int	take_forks_wo_lock_state(t_params *params, t_philo_info *philo)
{
	const size_t	first_take = philo->first_take;
	const size_t	second_take = philo->second_take;

//	pthread_mutex_lock(&params->lock_state);
	const size_t	left_idx = params->philo_info[philo->idx].left_philo_idx;
	const size_t	right_idx = params->philo_info[philo->idx].right_philo_idx;

	if (params->state[philo->idx] == STATE_THINKING \
	&& (params->state[left_idx] == STATE_WAITING \
	|| params->state[right_idx] == STATE_WAITING))
	{
		params->state[philo->idx] = STATE_THINKING;
		return (STATE_THINKING);
	}

	params->state[philo->idx] = STATE_WAITING;

	if (is_philo_can_eating_wo_lock_state(params, philo->idx))
//	if (params->state[philo->left_philo_idx] != STATE_EATING \
//	&& params->state[philo->right_philo_idx] != STATE_EATING)
	{
//		printf("take fork(%zu), %s\n", philo->idx, get_state_str(params->state[philo->idx]));
		params->state[philo->idx] = STATE_EATING;
		printf("\n");
		printf("[#Take] %zu-1 will:%zu\n", philo->idx, first_take);
		pthread_mutex_lock(&params->lock_each_fork[first_take]);
		printf("[#Take] %zu-2 take:%zu\n", philo->idx, first_take);
		printf("[#Take] %zu-3 will:%zu\n", philo->idx, second_take);
		pthread_mutex_lock(&params->lock_each_fork[second_take]);
		printf("[#Take] %zu-4 take:%zu\n", philo->idx, second_take);
		printf("\n");

		return (STATE_EATING);
	}
	return (STATE_WAITING);

//	pthread_mutex_unlock(&params->lock_state);

//	debug_print_state_w_lock(params, philo->idx);

	if (params->state[philo->idx] == STATE_EATING)
		return (SUCCESS);


	while (params->state[philo->idx] == STATE_HUNGRY) //wait
		if (params->is_died)
			return (PHILO_DIED);

//	printf("\n");
//	printf("[#Take] %zu-1 will:%zu(%s)\n", philo->idx, first_take, controlled);
	pthread_mutex_lock(&params->lock_each_fork[first_take]);
//	printf("[#Take] %zu-2 take:%zu(%s)\n", philo->idx, first_take, controlled);
//	printf("[#Take] %zu-3 will:%zu(%s)\n", philo->idx, second_take, controlled);
	pthread_mutex_lock(&params->lock_each_fork[second_take]);
//	printf("[#Take] %zu-4 take:%zu(%s)\n", philo->idx, second_take, controlled);
//	printf("\n");

	pthread_mutex_lock(&params->lock_state);
	params->state[philo->idx] = STATE_EATING;
	pthread_mutex_unlock(&params->lock_state);

	debug_print_state_w_lock(params, philo->idx);
	return (SUCCESS);
}

int	put_forks_wo_lock_state(t_params *params, t_philo_info *philo)
{
	debug_print_state_w_lock(params, philo->idx);

//	pthread_mutex_lock(&params->lock_state);

//	printf("\n");
//	printf("  [#Put] %zu-1 will %zu\n", philo->idx, philo->first_take);
	pthread_mutex_unlock(&params->lock_each_fork[philo->first_take]);
//	printf("  [#Put] %zu-2 put  %zu\n", philo->idx, philo->first_take);

//	printf("  [#Put] %zu-3 will %zu\n", philo->idx, philo->second_take);
	pthread_mutex_unlock(&params->lock_each_fork[philo->second_take]);
//	printf("  [#Put] %zu-4 put  %zu\n", philo->idx, philo->second_take);
//	printf("\n");

	if (params->state[philo->idx] == STATE_DIED)
		return (STATE_DIED);

	if (params->is_died)
		return (STATE_THINKING);

	params->state[philo->idx] = STATE_THINKING;

	if (params->state[philo->left_philo_idx] == STATE_WAITING \
	&& is_philo_can_eating_wo_lock_state(params, philo->left_philo_idx))
	{
		params->state[philo->left_philo_idx] = \
		take_forks_wo_lock_state(params, &params->philo_info[philo->left_philo_idx]);
	}
	if (params->state[philo->right_philo_idx] == STATE_WAITING \
	&& is_philo_can_eating_wo_lock_state(params, philo->right_philo_idx))
	{
		params->state[philo->right_philo_idx] = \
		take_forks_wo_lock_state(params, &params->philo_info[philo->right_philo_idx]);
	}

//	if (params->state[philo->left_philo_idx] == STATE_THINKING)
//	{
//		params->state[philo->left_philo_idx] = STATE_HUNGRY;
//	}
//	if (params->state[philo->right_philo_idx] == STATE_THINKING)
//	{
//		params->state[philo->right_philo_idx] = STATE_HUNGRY;
//	}

	//	pthread_mutex_unlock(&params->lock_state);

//	printf("\n");
//	printf("  [#Put] %zu-1 put %zu\n", philo->idx, philo->first_take);
//	printf("  [#Put] %zu-2 put %zu\n", philo->idx, philo->second_take);
//	printf("\n");

	return (STATE_THINKING);
}


