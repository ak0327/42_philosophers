/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/30 19:32:01 by takira            #+#    #+#             */
/*   Updated: 2022/12/30 19:32:03 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <errno.h>
# include <limits.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>

# include <unistd.h>

# define PASS	1
# define FAIL	0

# define NUM_OF_PHILOS_IDX		1
# define TIME_TO_DIE_IDX		2
# define TIME_TO_EAT_IDX		3
# define TIME_TO_SLEEP_IDX		4
# define MUST_EAT_TIMES_IDX		5

# define ERROR_MSG_NUM_OF_PHILOS	"1 <= NumOfPhilos <= 200"
# define ERROR_MSG_TIME_TO_DIE		"60 <= TimeToDie"
# define ERROR_MSG_TIME_TO_EAT		"60 <= TimeToEat"
# define ERROR_MSG_TIME_TO_SLEEP	"60 <= TimeToSleep"
# define ERROR_MSG_MUST_EAT_TIMES	"0 <= MustEatTimes"

typedef struct s_philo		t_info;
typedef enum s_input_type	t_input_type;

struct s_philo
{
	int	num_of_philo;
	int	time_to_die;
	int time_to_eat;
	int time_to_sleep;
	int must_eat_times;
};

enum s_input_type
{
	E_NUM_OF_PHILOS,
	E_TIME_TO_DIE,
	E_TIME_TO_EAT,
	E_TIME_TO_SLEEP,
	E_MUST_EAT_TIMES
};

/* libs */
int	ft_atoi(const char *str, bool *is_success);
char	*ft_strchr(const char *s, int c);
size_t	ft_strlen_ns(const char *s);
int		ft_isdigit(int c);

#endif
