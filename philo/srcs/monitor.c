/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/24 11:11:10 by takira            #+#    #+#             */
/*   Updated: 2023/02/24 15:35:33 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*monitor(void *v_params)
{
	t_params	*params;
	int			ret_value;

	ret_value = SUCCESS;
	params = (t_params *)v_params;
	while (ret_value == SUCCESS || ret_value == CONTINUE)
	{
		usleep(500);
		ret_value = is_some_philo_died(params);
		ret_value = is_meet_must_eat_times(params, ret_value);
	}
	return (NULL);
}
