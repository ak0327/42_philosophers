/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/30 19:32:01 by takira            #+#    #+#             */
/*   Updated: 2023/02/23 11:49:11 by takira           ###   ########.fr       */
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
# include <sys/time.h>
# include <unistd.h>

/* return value */
# define SUCCESS				0
# define FAILURE				1
# define PROCESS_ERROR			2
# define CONTINUE				3
# define BREAK					4

# define PHILO_ALIVE			0
# define PHILO_DIED				6

/* print_err_msg */
# define INVALID_ARG_COUNT		10
# define INVALID_NUM_OF_PHILOS	11
# define INVALID_TIME_TO_DIE	12
# define INVALID_TIME_TO_EAT	13
# define INVALID_TIME_TO_SLEEP	14
# define INVALID_MUST_EAT_TIMES	15

/* index */
# define NUM_OF_PHILOS_IDX		1
# define TIME_TO_DIE_IDX		2
# define TIME_TO_EAT_IDX		3
# define TIME_TO_SLEEP_IDX		4
# define MUST_EAT_TIMES_IDX		5

# define TO_JST					9

/* message */
# define PRINT_FORK		"has taken a fork"
# define PRINT_EATING	"is eating"
# define PRINT_SLEEPING	"is sleeping"
# define PRINT_THINKING	"is thinking"
# define PRINT_DIED		"died"

/* philo state */
# define STATE_THINKING		0
# define STATE_HUNGRY		1
# define STATE_WAITING		2
# define STATE_EATING		3
# define STATE_SLEEPING		4
# define STATE_DIED			5
# define STATE_TERMINATED	6

/* fork state */
# define FORK_DIRTY	0
# define FORK_CLEAN	1

/* print color */
# define PRINT_COLOR_WHITE	"\x1b[00m"
# define PRINT_COLOR_RED	"\x1b[31m"
# define PRINT_COLOR_GREEN	"\x1b[32m"
# define PRINT_COLOR_YELLOW	"\x1b[33m"
# define PRINT_COLOR_BLUE	"\x1b[34m"
# define PRINT_COLOR_RESET	"\x1b[0m"

typedef struct s_params		t_params;
typedef struct s_philo_info	t_philo_info;
typedef struct s_stack_elem	t_stack;

typedef enum e_input_type	t_input_type;
typedef enum e_print_type	t_print_type;

struct s_params
{
	size_t			num_of_philos;
	time_t			time_to_die;
	time_t			time_to_eat;
	time_t			time_to_sleep;
	ssize_t			must_eat_times;
	int				is_died;
	ssize_t			died_idx;
	t_philo_info	*philo_info;
	pthread_t		*philo_tid;
	ssize_t			*prev_used_by;
	pthread_mutex_t	*fork_mutex;
	pthread_mutex_t	*prev_used_mutex;
	pthread_mutex_t	print_mutex;
	pthread_mutex_t	died_mutex;
};

struct s_philo_info
{
	size_t			idx;
	size_t			first_take;
	size_t			second_take;
	struct timeval	start_time;
	size_t			eat_times;
	pthread_mutex_t	philo_mutex;
	bool			is_meet_eat_times;
	t_params		*params_ptr;
};

struct s_stack_elem
{
	size_t	idx;
	t_stack	*prev;
	t_stack	*next;
};

enum e_print_type
{
	TYPE_FORK,
	TYPE_EATING,
	TYPE_SLEEPING,
	TYPE_THINKING,
	TYPE_DIED
};

enum e_input_type
{
	TYPE_NUM_OF_PHILO,
	TYPE_TIME_TO_DIE,
	TYPE_TIME_TO_EAT,
	TYPE_TIME_TO_SLEEP,
	TYPE_MUST_EAT_TIMES
};

/* ------------ */
/*     philo    */
/* ------------ */
/* params_init.c */
int				init_params(int argc, char **argv, t_params **params);

/* params_destroy.c */
int				destroy_params(t_params *params);
void			free_params(t_params **params);

/* init_args.c */
int				get_input_args(char **argv, t_params *params);

/* handle_forks.c */
int				take_forks(t_philo_info *philo);
int				put_forks(t_philo_info *philo, int prev_ret_val);

/* is_died.c */
int				is_some_philo_died(t_params *paramsd);
int				get_is_died(t_params *params, ssize_t *idx, int prev_ret_val);

/* philo_status.c */
int				is_meet_must_eat_times(t_params *params, int prev_ret_val);
bool			get_is_meet_must_eat_times(t_philo_info *philo);
bool			get_is_meet_eat_times(t_philo_info *philo);
int				is_each_philo_meet_eat_times(\
t_philo_info *philo, int prev_ret_val);

/* print_console.c */
int				print_msg(size_t idx, t_print_type type, t_params *params);

/* routine.c */
void			*routine(void *v_philo_info);

/* thread.c */
int				create_threads(t_params *params);
int				join_threads(t_params *params);

/* time.c */
time_t			get_unix_time_ms(void);
void			print_timestamp(void);
struct timeval	get_start_time(t_philo_info *philo);
time_t			get_delta_time(struct timeval now_tv, struct timeval start_tv);

/* exit.c */
int				print_err_msg_and_free(int err, t_params *params, int ret);

/* debug_print.c */
void			print_eat_times(t_params *params);

/* ------------ */
/*     libs     */
/* ------------ */
// libft
int				ft_atoi(const char *str, bool *is_success);
int				ft_isdigit(int c);
size_t			ft_strlen_ns(const char *s);
char			*ft_strchr(const char *s, int c);
int				ft_isspace(char c);
long long		ft_strtoll(char *num, bool *is_success);

// stack
void			add_left(t_stack *elem, t_stack **stk);
void			add_right(t_stack *elem, t_stack **stk);
t_stack			*pop_left(t_stack **stk);
t_stack			*pop_right(t_stack **stk);
size_t			get_stack_size(t_stack *stk);
t_stack			*create_stack_elem(size_t idx);
t_stack			*get_last_elem(t_stack *elem);
void			ft_stack_clear(t_stack **stk);
void			swap(t_stack **stack);

// math
size_t			min_size(size_t a, size_t b);
size_t			max_size(size_t a, size_t b);
size_t			abs_size(size_t a, size_t b);

#endif