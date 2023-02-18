# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/12/30 19:31:34 by takira            #+#    #+#              #
#    Updated: 2022/12/30 19:31:36 by takira           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	= philo

CC			= cc
CFLAGS		= -Wall -Wextra -Werror -MMD -MP
#-g -fsanitize=address -fsanitize=undefined -fsanitize=thread

VPATH	= $(SRC_DIR) $(LIB_DIR)

SRC_DIR	= ./srcs
SRC		= main.c \
		  init_args.c \
		  init_params.c \
		  create_and_terminate_thread.c \
		  routine.c \
		  time.c \
		  print_console.c \
		  exit.c \

SRCS	= $(addprefix $(SRC_DIR)/, $(SRC))

LIB_DIR	= ./libs
LIB		= ft_atoi.c \
		  ft_isdigit.c \
		  ft_isspace.c \
		  ft_strchr.c \
		  ft_strlen.c \
		  ft_strtoll.c \
		  ft_stack.c \
		  ft_stack_operation.c \

LIBS	= $(addprefix $(LIB_DIR)/, $(LIB))

OBJ_DIR	= ./objs
OBJS	= $(addprefix $(OBJ_DIR)/, $(SRC:%.c=%.o)) $(addprefix $(OBJ_DIR)/, $(LIB:%.c=%.o))

DEPS	= $(SRCS:%.c:%.d) $(LIBS:%.c:%.d)

INCLUDE_DIR	= ./includes
IFLAGS		= $(addprefix -I, $(INCLUDE_DIR))


$(NAME)	: $(OBJS)
	$(CC) $(CFLAGS) $(DEPS_FLAG) $(OBJS) -o $@

$(OBJ_DIR)/%.o: %.c
	@mkdir -p $$(dirname $@)
	$(CC) $(CFLAGS) $(IFLAGS) -c $< -o $@

all		: $(NAME)

clean	:
	rm -rf $(OBJ_DIR)

fclean	: clean
	rm -f $(NAME)

re		: fclean all

bonus	:

norm	:
	@norminette -v
	@norminette $(SRC_DIR) $(LIB_DIR) $(INCLUDE_DIR)

.PHONY	: all clean fclean re bonus

-include	$(DEPS)