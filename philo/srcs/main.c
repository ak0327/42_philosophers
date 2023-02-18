/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/30 19:32:16 by takira            #+#    #+#             */
/*   Updated: 2023/02/18 17:10:34 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	init_philo(t_params **params, t_each_philo **philo)
{
	size_t	idx;
	t_each_philo	*last;
	t_each_philo	*new;

	last = NULL;
	idx = 0;
	while (idx < (*params)->num_of_philos)
	{
		new = (t_each_philo *)malloc(sizeof(t_each_philo));
		if (!new)
			return (PROCESS_ERROR);
		new->idx = idx;
		new->params = *params;
		new->std_time = 0;
		new->wait = create_stack_elem(idx);
		if (!new->wait)
		{
			free(new);
			return (PROCESS_ERROR);
		}
		new->next = NULL;
		if (!last)
			*philo = new;
		if (last)
			last->next = new;
		last = new;
		idx++;
	}
	return (SUCCESS);
}

t_each_philo	*create_each_philo_info(t_params **params)
{
	t_each_philo	*philo_info;
	size_t			idx;

	philo_info = (t_each_philo *)malloc(sizeof(t_each_philo) * ((*params)->num_of_philos + 1));
	if (!philo_info)
		return (NULL);
	memset(philo_info, 0, sizeof(t_each_philo) * ((*params)->num_of_philos + 1));
	idx = 0;
	while (idx < (*params)->num_of_philos)
	{
		philo_info[idx].idx = idx;
		philo_info[idx].params = *params;
		philo_info[idx].wait = create_stack_elem(idx);
		philo_info[idx].is_allowed = false;
		idx++;
	}
	return (philo_info);
}

int	check_forks_available(t_params *params, size_t idx)
{
	const size_t	num_of_philos = params->num_of_philos;
	size_t			left_idx;
	size_t			right_idx;

	left_idx = idx % num_of_philos;
	right_idx = (idx + 1) % num_of_philos;
	return (params->forks[left_idx] == 0 && params->forks[right_idx] == 0);
}

void	print_waiting(t_stack *stack)
{
	printf("[#DEBUG]waiting:: ");
	while (stack)
	{
		printf("%zu", stack->idx);
		stack = stack->next;
		if (stack)
			printf(" -> ");
		else
			printf("\n");
	}
}

void	decide_fork_using_philo(t_params *params)
{
	t_stack	*wait_stack;
	t_stack	*popped;

	wait_stack = NULL;

	pthread_mutex_lock(&params->lock_fork);
	while (params->wait_queue)
	{
		popped = pop_left(&params->wait_queue);
		if (check_forks_available(params, popped->idx))
		{
//			printf("available idx:%zu\n", popped->idx);
			take_forks(params, popped->idx);
			(&params->philo_info[popped->idx])->is_allowed = true;
			break ;
		}
		else
			add_right(popped, &wait_stack);
	}
	pthread_mutex_unlock(&params->lock_fork);
	while (wait_stack)
	{
		popped = pop_right(&wait_stack);
		add_left(popped, &params->wait_queue);
	}
}

int	main(int argc, char **argv)
{
	t_params		*params;
	t_each_philo	*philo;
	int				ret_value;

	params = NULL;
	philo = NULL;
	ret_value = init_params(argc, argv, &params);
	if (ret_value != SUCCESS)
		return (print_err_msg_and_free_allocs(ret_value, params, philo, EXIT_FAILURE));

	ret_value = init_philo(&params, &philo);
	if (ret_value != SUCCESS)
		return (print_err_msg_and_free_allocs(ret_value, params, philo, EXIT_FAILURE));

	params->philo_info = create_each_philo_info(&params);
	if (!params->philo_info)
		return (print_err_msg_and_free_allocs(ret_value, params, philo, EXIT_FAILURE));

	ret_value = create_threads(params);
	if (ret_value != SUCCESS)
		return (print_err_msg_and_free_allocs(ret_value, params, philo, EXIT_FAILURE));

	while (!params->is_died)
	{
		pthread_mutex_lock(&params->lock_waiter);
		decide_fork_using_philo(params);
		pthread_mutex_unlock(&params->lock_waiter);
//		usleep(100);
	}

//	ret_value = monitor_philos(params);
//	if (ret_value != SUCCESS)
//		return (print_err_msg_and_free_allocs(ret_value, params, philo, EXIT_FAILURE));

	ret_value = terminate_threads(params);
	if (ret_value != SUCCESS)
		return (print_err_msg_and_free_allocs(ret_value, params, philo, EXIT_FAILURE));

	free_allocs(params, philo);

//	system("leaks -q philo");
	return (EXIT_SUCCESS);
}
