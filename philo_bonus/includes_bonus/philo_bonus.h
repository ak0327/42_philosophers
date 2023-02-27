/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/24 16:43:10 by takira            #+#    #+#             */
/*   Updated: 2023/02/27 11:13:06 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_BONUS_H
# define PHILO_BONUS_H

# include <limits.h>
# include <pthread.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/time.h>
# include <unistd.h>
# include <semaphore.h>
# include <signal.h>

/* return value */
# define SUCCESS		0
# define FAILURE		1
# define PROCESS_ERROR	2
# define CONTINUE		3
# define BREAK			4

# define PHILO_ALIVE	0
# define PHILO_DIED		6

/* fork */
# define CHILD_PROCESS	0

/* semaphore */
# define SEM_FORKS	"/sem_forks"
# define SEM_WAITER	"/sem_waiter"
# define SEM_PHILO	"/sem_philo_"
# define SEM_PRINT	"/sem_print"

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
# define PRINT_FORK			"has taken a fork"
# define PRINT_EATING		"is eating"
# define PRINT_SLEEPING		"is sleeping"
# define PRINT_THINKING		"is thinking"
# define PRINT_DIED			"died"
# define PRINT_SIM_START	"<SIMULATION START>"

typedef struct s_info		t_info;
typedef struct s_philo_info	t_philo_info;
typedef struct s_stack_elem	t_stack;
typedef struct timeval		t_timeval;

typedef enum e_input_type	t_input_type;
typedef enum e_print_type	t_print_type;
typedef enum e_philo_status	t_status;

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
	TYPE_DIED,
	TYPE_SIM_START
};

enum e_input_type
{
	TYPE_NUM_OF_PHILO,
	TYPE_TIME_TO_DIE,
	TYPE_TIME_TO_EAT,
	TYPE_TIME_TO_SLEEP,
	TYPE_MUST_EAT_TIMES
};

enum e_philo_status
{
	PH_TAKE_FORK,
	PH_EATING,
	PH_SLEEPING,
	PH_THINKING,
	PH_DIED,
	PH_SATISFIED
};

struct s_info
{
	size_t			num_of_philos;
	time_t			time_to_die;
	time_t			time_to_eat;
	time_t			time_to_sleep;
	ssize_t			must_eat_times;

	int				is_died;

	sem_t			*sem_forks;
	sem_t			*sem_waiter;

	sem_t			*sem_print;

	t_philo_info	*philo_info;
	pthread_t		*philo_tid;

	time_t			start_time;
};

struct s_philo_info
{
	pthread_t		philo_tid;
	pthread_t		monitor_tid;

	size_t			idx;

	t_status		status;

	time_t			start_time;
	pid_t			pid;

	size_t			eat_cnt;
	bool			is_satisfied;
	bool			is_died;
	bool			is_died_printed;

	char			*sem_name;
	sem_t			*sem_philo;
	t_info			*info_ptr;
};

/* ------------ */
/*     philo    */
/* ------------ */

/* params_init.c */
int			init_info(int argc, char **argv, t_info **info);

/* params_destroy.c */
void		free_info(t_info **info);
int			destroy_info(t_info *info);

/* get_input_args.c */
int			get_input_args(char **argv, t_info *info);

/* exit.c */
int			print_err_msg_and_free(int err, t_info *info, int ret);

/* routine.c */
void		*routine(t_philo_info *philo);

/* check_philo_status.c */
bool		get_is_died(t_philo_info *philo, bool *is_died_printed);
size_t		get_eat_cnt(t_philo_info *philo);
int			check_philo_died(t_philo_info *philo, time_t now_time);
int			check_continue(t_philo_info *philo);

/* time.c */
void		sleep_ms(time_t time_ms);
time_t		get_delta_time_ms(time_t now_ms, time_t start_ms);
time_t		get_unix_time_ms(void);

/* handle_forks.c */
int			take_forks(t_philo_info *philo);
int			put_forks_and_update_eat_times(t_philo_info *philo);

/* monitor.c */
//void		*monitor(t_philo_info *philo);
int			monitor(t_philo_info *philo);

/* start_routine.c */
int			start_routine(t_info *info);

/* print_console.c */
time_t		get_print_time(t_philo_info *philo, t_print_type type);
int			print_msg(t_print_type type, t_philo_info *philo);
int			print_msg_consider_died(t_philo_info *philo, t_print_type type);

/* ------------ */
/*     libs_bonus     */
/* ------------ */
// libft
int			ft_atoi(const char *str, bool *is_success);
int			ft_isdigit(int c);
size_t		ft_strlen_ns(const char *s);
char		*ft_strchr(const char *s, int c);
int			ft_isspace(char c);
long long	ft_strtoll(char *num, bool *is_success);
size_t		ft_strlcat_ns(char *dst, const char *src, size_t dstsize);
void		*ft_calloc(size_t count, size_t size);
char		*ft_itoa(int n);

// stack
void		add_left(t_stack *elem, t_stack **stk);
void		add_right(t_stack *elem, t_stack **stk);
t_stack		*pop_left(t_stack **stk);
t_stack		*pop_right(t_stack **stk);
size_t		get_stack_size(t_stack *stk);
t_stack		*create_stack_elem(size_t idx);
t_stack		*get_last_elem(t_stack *elem);
void		ft_stack_clear(t_stack **stk);
void		swap(t_stack **stack);

// math
size_t		min_size(size_t a, size_t b);
size_t		max_size(size_t a, size_t b);
size_t		abs_size(size_t a, size_t b);

#endif //PHILO_BONUS_H