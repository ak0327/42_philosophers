/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_params.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/17 10:00:01 by takira            #+#    #+#             */
/*   Updated: 2023/02/17 11:03:23 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"



int	init_params(int argc, char **argv, t_params *params)
{
	if (!(5 <= argc && argc <= 6))
		return (INVALID_ARG);
	params = (t_params *)malloc(sizeof(t_params));
	if (!params)
		return (PROCESS_ERROR);
	memset(params, 0, sizeof(t_params));
	if (get_input_args(argv, params) == FAILURE)
		return (FAILURE);

}
