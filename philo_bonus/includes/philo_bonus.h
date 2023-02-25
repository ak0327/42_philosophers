/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/24 16:43:10 by takira            #+#    #+#             */
/*   Updated: 2023/02/24 21:49:21 by takira           ###   ########.fr       */
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
# define PRINT_FORK			"has taken a fork"
# define PRINT_EATING		"is eating"
# define PRINT_SLEEPING		"is sleeping"
# define PRINT_THINKING		"is thinking"
# define PRINT_DIED			"died"
# define PRINT_SIM_START	"simulation start"

typedef struct s_info		t_info;
typedef struct s_philo_info	t_philo_info;
typedef struct s_stack_elem	t_stack;
typedef struct timeval		t_timeval;

typedef enum e_input_type	t_input_type;
typedef enum e_print_type	t_print_type;

struct s_info
{
	size_t			num_of_philos;
	time_t			time_to_die;
	time_t			time_to_eat;
	time_t			time_to_sleep;
	ssize_t			must_eat_times;

	int				is_died;
	ssize_t			died_idx;
	int				is_sim_fin;

	sem_t			*sem_forks;
	sem_t			*sem_waiter;


	t_philo_info	*philo_info;
	pthread_t		*philo_tid;

	pthread_t		monitor_tid;

	ssize_t			*prev_used_by;

	time_t			start_time;
};

struct s_philo_info
{
	size_t			idx;

	time_t			start_time;
	size_t			eat_times;

	t_info			*info_ptr;


	size_t			first_take;
	size_t			second_take;
	bool			is_meet_eat_times;
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


/* ------------ */
/*     philo    */
/* ------------ */

/* params_init.c */
int		init_info(int argc, char **argv, t_info **info);

/* params_destroy.c */
void	free_info(t_info **info);
int		destroy_info(t_info *info);

/* get_input_args.c */
int		get_input_args(char **argv, t_info *info);

/* exit.c */
int		print_err_msg_and_free(int err, t_info *info, int ret);

/* ------------ */
/*     libs     */
/* ------------ */
// libft
int			ft_atoi(const char *str, bool *is_success);
int			ft_isdigit(int c);
size_t		ft_strlen_ns(const char *s);
char		*ft_strchr(const char *s, int c);
int			ft_isspace(char c);
long long	ft_strtoll(char *num, bool *is_success);

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