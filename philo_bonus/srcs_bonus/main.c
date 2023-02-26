/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/24 16:51:38 by takira            #+#    #+#             */
/*   Updated: 2023/02/26 19:36:09 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

/*
 index of philo and forks are following:
  fork   4forks in middle of the table
  philo  0  1  2  3
*/

int	main(int argc, char **argv)
{
	t_info	*info;
	int		ret_value;

	info = NULL;
	ret_value = init_info(argc, argv, &info);
	if (ret_value != SUCCESS)
		return (print_err_msg_and_free(ret_value, info, EXIT_FAILURE));
	ret_value = start_routine(info);
	if (ret_value != SUCCESS)
		return (print_err_msg_and_free(ret_value, info, EXIT_FAILURE));
	ret_value = destroy_info(info);
	if (ret_value != SUCCESS)
		return (print_err_msg_and_free(ret_value, info, EXIT_FAILURE));
	return (EXIT_SUCCESS);
}

#ifdef LEAKS

__attribute__((destructor))
static void	destructor(void)
{
	system("leaks -q philo_bonus");
}

#endif
