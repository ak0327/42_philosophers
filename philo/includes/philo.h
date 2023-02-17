/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/30 19:32:01 by takira            #+#    #+#             */
/*   Updated: 2023/02/17 21:00:38 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <errno.h>
# include <limits.h>
# include <pthread.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>

# define SUCCESS				0
# define FAILURE				1
# define PROCESS_ERROR			2
# define INVALID_ARG_COUNT		3
# define INVALID_NUM_OF_PHILOS	4
# define INVALID_TIME_TO_DIE	5
# define INVALID_TIME_TO_EAT	6
# define INVALID_TIME_TO_SLEEP	7
# define INVALID_MUST_EAT_TIMES	8

# define NUM_OF_PHILOS_IDX		1
# define TIME_TO_DIE_IDX		2
# define TIME_TO_EAT_IDX		3
# define TIME_TO_SLEEP_IDX		4
# define MUST_EAT_TIMES_IDX		5

# define JST					9

# define PRINT_FORK		"has taken a fork"
# define PRINT_EATING	"is eating"
# define PRINT_SLEEPING	"is sleeping"
# define PRINT_THINKING	"is thinking"
# define PRINT_DIED		"died"

# define PHILO_DEAD		0
# define PHILO_ALIVE	1

# define CHECK_TIME_SPAN_MS		100

typedef struct s_params		t_params;
typedef struct s_args		t_args;
typedef struct s_philo		t_philo;
typedef struct s_time		t_time;

typedef enum s_input_type	t_input_type;
typedef enum s_print_type	t_print_type;

struct s_philo
{
	size_t		philo_idx;
	t_params	*params;
	time_t		start_time;
	t_philo		*next;
};

struct s_params
{
	// args
	size_t			num_of_philos;
	time_t			time_to_die;
	time_t			time_to_eat;
	time_t			time_to_sleep;
	ssize_t			must_eat_times;

	// philo
	pthread_t		*philo_no;

	// lock_fork
//	pthread_mutex_t	*forks;

	int				*forks;

	pthread_mutex_t	lock_fork;

	// lock_waiter
	pthread_mutex_t	lock_waiter;

	// print
	pthread_mutex_t	lock_print;

	ssize_t			*eat_times;
	bool			is_died;
};

enum s_print_type
{
	TYPE_FORK,
	TYPE_EATING,
	TYPE_SLEEPING,
	TYPE_THINKING,
	TYPE_DIED
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
int			init_params(int argc, char **argv, t_params **params);
int			get_input_args(char **argv, t_params *params);
int			init_thread(t_params *params);

int			create_threads(t_params *params, t_philo *philo);
int			monitor_philos(t_params *params);
int			terminate_threads(t_params *params);

void		free_allocs(t_params *params, t_philo *philo);
int			print_err_msg_and_free_allocs(int err, t_params *params, t_philo *philo, int ret);


/* libs */
int			ft_atoi(const char *str, bool *is_success);
int			ft_isdigit(int c);
size_t		ft_strlen_ns(const char *s);
char		*ft_strchr(const char *s, int c);
int			ft_isspace(char c);
long long	ft_strtoll(char *num, bool *is_success);

#endif
