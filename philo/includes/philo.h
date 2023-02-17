/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/30 19:32:01 by takira            #+#    #+#             */
/*   Updated: 2023/02/17 10:44:25 by takira           ###   ########.fr       */
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
# include <pthread.h>

# include <unistd.h>

# define SUCCESS		0
# define FAILURE		1
# define PROCESS_ERROR	2
# define INVALID_ARG	3


# define PHILO_DEAD		0
# define PHILO_ALIVE	1

# define NUM_OF_PHILOS_IDX		1
# define TIME_TO_DIE_IDX		2
# define TIME_TO_EAT_IDX		3
# define TIME_TO_SLEEP_IDX		4
# define MUST_EAT_TIMES_IDX		5

# define CHECK_TIME_SPAM_MS		100

# define ERROR_MSG_NUM_OF_PHILOS	"1 <= NumOfPhilos <= 200"
# define ERROR_MSG_TIME_TO_DIE		"60 <= TimeToDie"
# define ERROR_MSG_TIME_TO_EAT		"60 <= TimeToEat"
# define ERROR_MSG_TIME_TO_SLEEP	"60 <= TimeToSleep"
# define ERROR_MSG_MUST_EAT_TIMES	"0 <= MustEatTimes"

typedef struct s_params		t_params;
typedef struct s_args		t_args;
typedef struct s_philo		t_philo;
typedef enum s_input_type	t_input_type;

struct s_params
{
	// args
	size_t			num_of_philo;
	time_t			time_to_die;
	time_t			time_to_eat;
	time_t			time_to_sleep;
	ssize_t			must_eat_times;

	// philo
	pthread_t		*philo_no;

	// fork
	pthread_mutex_t	*forks;

	// waiter
	pthread_mutex_t	waiter;

	// print
	pthread_mutex_t	print_console;
};

struct s_args
{
	int	num_of_philo;
	int	time_to_die;
	int time_to_eat;
	int time_to_sleep;
	int must_eat_times;
};

struct s_philo
{
	// philo
	pthread_t	*no;

	// fork
	pthread_mutex_t	*forks;

	// waiter
	pthread_mutex_t	waiter;

	// print
	pthread_mutex_t	print_console;

};

enum s_input_type
{
	TYPE_NUM_OF_PHILO,
	TYPE_TIME_TO_DIE,
	TYPE_TIME_TO_EAT,
	TYPE_TIME_TO_SLEEP,
	TYPE_MUST_EAT_TIMES
};

/* philo */
int			init_params(int argc, char **argv, t_params *params);
int			get_input_args(char **argv, t_params *params);


void		print_error(int err);


/* libs */
int			ft_atoi(const char *str, bool *is_success);
int			ft_isdigit(int c);
size_t		ft_strlen_ns(const char *s);
char		*ft_strchr(const char *s, int c);
int			ft_isspace(char c);
long long	ft_strtoll(char *num, bool *is_success);

#endif
