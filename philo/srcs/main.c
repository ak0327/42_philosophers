/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/30 19:32:16 by takira            #+#    #+#             */
/*   Updated: 2023/02/27 14:34:55 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/*
 index of philo and forks are following:
  fork     0   1   2   3   0
  philo    ^ 0 ^ 1 ^ 2 ^ 3 ^
*/

int	main(int argc, char **argv)
{
	t_params	*params;
	int			ret_value;

	params = NULL;
	ret_value = init_params(argc, argv, &params);
	if (ret_value != SUCCESS)
		return (print_err_msg_and_free(ret_value, params, EXIT_FAILURE));
	ret_value = create_threads(params);
	if (ret_value != SUCCESS)
		return (print_err_msg_and_free(ret_value, params, EXIT_FAILURE));
	ret_value = join_threads(params);
	if (ret_value != SUCCESS)
		return (print_err_msg_and_free(ret_value, params, EXIT_FAILURE));
	ret_value = destroy_params(params);
	if (ret_value != SUCCESS)
		return (print_err_msg_and_free(ret_value, params, EXIT_FAILURE));
	return (EXIT_SUCCESS);
}

#ifdef LEAKS

__attribute__((destructor))
static void	destructor(void)
{
	system("leaks -q philo");
}

#endif