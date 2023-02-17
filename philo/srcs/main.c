/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/30 19:32:16 by takira            #+#    #+#             */
/*   Updated: 2023/02/17 19:55:25 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	init_philo(t_params **params, t_philo **philo)
{
	size_t	idx;
	t_philo	*last;
	t_philo	*new;

	last = NULL;
	idx = 0;
	while (idx < (*params)->num_of_philos)
	{
		new = (t_philo *)malloc(sizeof(t_philo));
		if (!new)
			return (PROCESS_ERROR);
		new->philo_idx = idx;
		new->params = *params;
		new->start_time = 0;
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

int	main(int argc, char **argv)
{
	t_params	*params;
	t_philo		*philo;
	int			ret_value;

	params = NULL;
	philo = NULL;
	ret_value = init_params(argc, argv, &params);
	if (ret_value != SUCCESS)
		return (print_err_msg_and_free_allocs(ret_value, params, philo, EXIT_FAILURE));

	ret_value = init_philo(&params, &philo);
	if (ret_value != SUCCESS)
		return (print_err_msg_and_free_allocs(ret_value, params, philo, EXIT_FAILURE));

	ret_value = create_threads(params, philo);
	if (ret_value != SUCCESS)
		return (print_err_msg_and_free_allocs(ret_value, params, philo, EXIT_FAILURE));

	ret_value = monitor_philos(params);
	if (ret_value != SUCCESS)
		return (print_err_msg_and_free_allocs(ret_value, params, philo, EXIT_FAILURE));

	ret_value = terminate_threads(params);
	if (ret_value != SUCCESS)
		return (print_err_msg_and_free_allocs(ret_value, params, philo, EXIT_FAILURE));

	free_allocs(params, philo);
	printf("\n");
	printf("\n");

//	system("leaks -q philo");
	return (EXIT_SUCCESS);
}
