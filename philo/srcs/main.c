/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/30 19:32:16 by takira            #+#    #+#             */
/*   Updated: 2023/02/22 10:17:03 by takira           ###   ########.fr       */
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
		return (print_err_msg_and_free_allocs(ret_value, params, EXIT_FAILURE));
	ret_value = create_threads(params);
	if (ret_value != SUCCESS)
		return (print_err_msg_and_free_allocs(ret_value, params, EXIT_FAILURE));

	while (true)
	{
		if (is_philo_died(params))
			break ;
//		pthread_mutex_lock(&params->died_mutex);
//		is_died = params->is_died;
//		pthread_mutex_unlock(&params->died_mutex);
//		if (is_died)
//			break ;
		if (is_meet_must_eat_times(params))
			break ;
		// check all philo meet must eat times
		usleep(500);
	}

	ret_value = terminate_threads(params);
	if (ret_value != SUCCESS)
		return (print_err_msg_and_free_allocs(ret_value, params, EXIT_FAILURE));
	ret_value = destroy_params(params);
	if (ret_value != SUCCESS)
		return (print_err_msg_and_free_allocs(ret_value, params, EXIT_FAILURE));
	return (EXIT_SUCCESS);
}

#ifdef LEKAKS
__attribute__((destructor))
static void	destructor(void)
{
	system("leaks -q philo");
}
#endif