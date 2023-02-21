/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/30 19:32:01 by takira            #+#    #+#             */
/*   Updated: 2023/02/21 18:15:03 by takira           ###   ########.fr       */
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

/* return value */
# define SUCCESS				0
# define FAILURE				1
# define PROCESS_ERROR			2
# define INVALID_ARG_COUNT		3
# define INVALID_NUM_OF_PHILOS	4
# define INVALID_TIME_TO_DIE	5
# define INVALID_TIME_TO_EAT	6
# define INVALID_TIME_TO_SLEEP	7
# define INVALID_MUST_EAT_TIMES	8
# define INTERRUPT				9
# define CONTINUE				10

/* index */
# define NUM_OF_PHILOS_IDX		1
# define TIME_TO_DIE_IDX		2
# define TIME_TO_EAT_IDX		3
# define TIME_TO_SLEEP_IDX		4
# define MUST_EAT_TIMES_IDX		5

# define JST					9

/* message */
# define PRINT_FORK		"has taken a fork"
# define PRINT_EATING	"is start_eating"
# define PRINT_SLEEPING	"is start_sleeping"
# define PRINT_THINKING	"is start_thinking"
# define PRINT_DIED		"died"

# define PHILO_DIED		-1
# define PHILO_ALIVE	1

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

/* parameter */
# define MONITOR_TIME_SPAN_US	5000

typedef struct s_params		t_params;
typedef struct s_philo_info	t_philo_info;
typedef struct s_stack_elem	t_stack;

typedef enum s_input_type	t_input_type;
typedef enum s_print_type	t_print_type;

struct s_params
{
	// args
	size_t			num_of_philos;
	time_t			time_to_die;
	time_t			time_to_eat;
	time_t			time_to_sleep;
	ssize_t			must_eat_times;

	// philo
	pthread_t		*thread_id;
	t_philo_info	*philo_info;

	int				*state;

	// fork
	ssize_t			*prev_used_by;
	ssize_t			*held_by;

	// died
	bool			is_died;
	ssize_t			died_idx;

	// mutex
	pthread_mutex_t	*fork_mutex;
	pthread_mutex_t	print_mutex;
	pthread_mutex_t	died_mutex;

	pthread_t		monitor;
	bool			is_continue_monitor;
};

struct s_philo_info
{
	size_t		idx;

	size_t		first_take;
	size_t		second_take;

	time_t		start_time;
	size_t		eat_times;

	bool		is_continue;

	t_params	*params_ptr;
};

struct s_stack_elem
{
	size_t	idx;
	t_stack	*prev;
	t_stack	*next;
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

int			create_threads(t_params *params);
int			monitor_philos(t_params *params);
int			terminate_threads(t_params *params);

void		free_params(t_params **params);
int			print_err_msg_and_free_allocs(int err, t_params *params, int ret);

time_t		get_unix_time_ms(void);
void		*routine(void *v_philo_info);

int			take_forks(t_philo_info *philo);
int			put_forks(t_philo_info *philo);
void		*monitor(void *v_params);


void		print_waiting(t_stack *stack);
void		print_msg(size_t idx, t_print_type type, time_t time, t_params *params);

int			check_philo_alive(t_params *params, size_t idx, time_t std_time);


void		debug_print_state_w_lock(t_params *params, size_t id);
void		debug_print_state_wo_lock(t_params *params, size_t id);
void		print_timestamp(void);
char		*get_state_str(int state);

int			destroy_params(t_params *params);

/* libs */
int			ft_atoi(const char *str, bool *is_success);
int			ft_isdigit(int c);
size_t		ft_strlen_ns(const char *s);
char		*ft_strchr(const char *s, int c);
int			ft_isspace(char c);
long long	ft_strtoll(char *num, bool *is_success);

void		add_left(t_stack *elem, t_stack **stk);
void		add_right(t_stack *elem, t_stack **stk);
t_stack		*pop_left(t_stack **stk);
t_stack		*pop_right(t_stack **stk);
size_t		get_stack_size(t_stack *stk);
t_stack		*create_stack_elem(size_t idx);
t_stack		*get_last_elem(t_stack *elem);
void		ft_stack_clear(t_stack **stk);
void		swap(t_stack **stack);

size_t		min_size(size_t a, size_t b);
size_t		max_size(size_t a, size_t b);
size_t		abs_size(size_t a, size_t b);

#endif
