/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_params.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/17 10:00:01 by takira            #+#    #+#             */
/*   Updated: 2023/02/17 22:08:39 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	init_params(int argc, char **argv, t_params **params)
{
	int	ret_value;

	if (!(5 <= argc && argc <= 6))
		return (INVALID_ARG_COUNT);
	*params = (t_params *)malloc(sizeof(t_params));
	if (!*params)
		return (PROCESS_ERROR);

	memset(*params, 0, sizeof(t_params));

	ret_value = get_input_args(argv, *params);
	if (ret_value !=SUCCESS)
	{
		free(*params);
		return (ret_value);
	}
	ret_value = init_thread(*params);
	if (ret_value != SUCCESS)
	{
		free(*params);
		return (ret_value);
	}
	if ((*params)->num_of_philos % 2 == 0)
		(*params)->wait_philo = -1;
	else
		(*params)->wait_philo = 0;
	return (ret_value);
}
