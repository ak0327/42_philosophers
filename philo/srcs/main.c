/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/30 19:32:16 by takira            #+#    #+#             */
/*   Updated: 2023/02/20 00:29:26 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
t_philo_info	*create_each_philo_info(t_params **params)
{
	t_philo_info	*philo_info;
	size_t			idx;
	size_t			num_of_philos;

	num_of_philos = (*params)->num_of_philos;
	philo_info = (t_philo_info *)malloc(sizeof(t_philo_info) * (num_of_philos + 1));
	if (!philo_info)
		return (NULL);
	memset(philo_info, 0, sizeof(t_philo_info) * (num_of_philos + 1));
	idx = 0;
	while (idx < num_of_philos)
	{
		philo_info[idx].idx = idx;
		philo_info[idx].params = *params;
		philo_info[idx].wait_info = create_stack_elem(idx);
		philo_info[idx].is_allowed = false;

		/* fork idx */
		philo_info[idx].left_fork_idx = idx;
		philo_info[idx].right_fork_idx = (idx + 1) % num_of_philos;

		philo_info[idx].first_take = philo_info[idx].left_fork_idx;
		philo_info[idx].second_take = philo_info[idx].right_fork_idx;

//		if (idx == 0)
//		{
//			philo_info[idx].first_take = philo_info[idx].right_fork_idx;
//			philo_info[idx].second_take = philo_info[idx].left_fork_idx;
//		}
//		if (idx % 2 == 1)
//		{
//			philo_info[idx].first_take = (idx + 1) % num_of_philos;
//			philo_info[idx].second_take = idx;
//		}

		/* left and right philo idx */

		philo_info[idx].left_philo_idx = (idx + num_of_philos - 1) % num_of_philos;
		philo_info[idx].right_philo_idx = (idx + 1) % num_of_philos;

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
	return (params->fork_arr[left_idx] == 0 && params->fork_arr[right_idx] == 0);
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
			take_forks(params, &params->philo_info[popped->idx]);
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

int	monitor(t_params *params)
{
	const time_t	now_time = get_unix_time_ms();
	const time_t	time_to_die = params->time_to_die;
	time_t			start_time;
	size_t			idx;

	idx = 0;
	while (idx < params->num_of_philos)
	{
		start_time = params->philo_info[idx].start_time;
		if (now_time - start_time >= time_to_die)
		{
			pthread_mutex_lock(&params->lock_died);
			params->is_died = true;
			params->died_philo = (ssize_t)idx;
			pthread_mutex_lock(&params->lock_print);
			printf("\x1b[31mmonitor\x1b[0m \x1b[48;5;%03zum%zu\x1b[0m \x1b[31mis died\x1b[0m\n", idx % 255, idx);
			pthread_mutex_unlock(&params->lock_print);

			pthread_mutex_unlock(&params->lock_died);
			return (PHILO_DIED);
		}
		idx++;
	}
	return (PHILO_ALIVE);
}

// fork     0   1   2   3   0
// philo  3 ^ 0 ^ 1 ^ 2 ^ 3 ^

int	main(int argc, char **argv)
{
	t_params		*params;
	int				ret_value;

	params = NULL;
	ret_value = init_params(argc, argv, &params);
	if (ret_value != SUCCESS)
		return (print_err_msg_and_free_allocs(ret_value, params, EXIT_FAILURE));

	params->philo_info = create_each_philo_info(&params);
	if (!params->philo_info)
		return (print_err_msg_and_free_allocs(ret_value, params, EXIT_FAILURE));

	ret_value = create_threads(params);
	if (ret_value != SUCCESS)
		return (print_err_msg_and_free_allocs(ret_value, params, EXIT_FAILURE));


	while (true)
	{
		if (monitor(params) == PHILO_DIED)
			break ;
		usleep(500);
//		pthread_mutex_lock(&params->lock_waiter);
//		decide_fork_using_philo(params);
//		pthread_mutex_unlock(&params->lock_waiter);
	}

	ret_value = terminate_threads(params);
	if (ret_value != SUCCESS)
		return (print_err_msg_and_free_allocs(ret_value, params, EXIT_FAILURE));

	free_allocs(params);

	return (EXIT_SUCCESS);
}

__attribute__((destructor))
static void	destructor(void)
{
	system("leaks -q philo");
}