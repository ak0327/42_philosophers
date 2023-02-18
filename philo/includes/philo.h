/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/30 19:32:01 by takira            #+#    #+#             */
/*   Updated: 2023/02/18 10:58:29 by takira           ###   ########.fr       */
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
# define PRINT_EATING	"is eating"
# define PRINT_SLEEPING	"is sleeping"
# define PRINT_THINKING	"is thinking"
# define PRINT_DIED		"died"

# define PHILO_DIED		0
# define PHILO_ALIVE	1

/* parameter */
# define CHECK_TIME_SPAN_MS		100

typedef struct s_params		t_params;
typedef struct s_args		t_args;
typedef struct s_each_philo	t_each_philo;
typedef struct s_time		t_time;
typedef struct s_stack_elem	t_stack;

typedef enum s_input_type	t_input_type;
typedef enum s_print_type	t_print_type;

struct s_each_philo
{
	t_params		*params;

	size_t			idx;
	time_t			std_time;

	t_stack			*wait;
	bool			is_allowed;

	t_each_philo	*next;
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
	pthread_mutex_t	lock_waiter;
	pthread_mutex_t	lock_print;
	pthread_mutex_t	lock_died;
	pthread_mutex_t	lock_eat_times;

	ssize_t			*eat_times;
	time_t			died_time;
	ssize_t			wait_philo_idx;

	bool			is_died;
	ssize_t			died_philo;
	t_stack			*wait_queue;
	t_each_philo	*philo_info;

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
int			init_thread(t_params *params);

int			create_threads(t_params *params);
int			monitor_philos(t_params *params);
int			terminate_threads(t_params *params);

void		free_allocs(t_params *params, t_each_philo *philo);
int			print_err_msg_and_free_allocs(int err, t_params *params, t_each_philo *philo, int ret);

time_t	get_unix_time_ms(void);
void	*do_routine(void *v_philo);

void	print_msg(size_t idx, t_print_type type, time_t time, t_params *params);

int	take_forks(t_params *params, size_t idx);

time_t	get_unix_time_ms(void);


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

void	print_waiting(t_stack *stack);


#endif
