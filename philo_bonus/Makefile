
NAME	= philo_bonus

CC		= cc
CFLAGS	= -Wall -Wextra -Werror -MMD -MP

ifdef WITH_SANITIZE
CFLAGS 	+= -g -fsanitize=thread
endif

ifdef WITH_LEAKS
	CFLAGS	+= -D LEAKS
endif

VPATH	= $(SRC_DIR) $(LIB_DIR)

SRC_DIR	= ./srcs
SRC		= main.c \

SRCS	= $(addprefix $(SRC_DIR), $(SRC))

LIB_DIR	= ./libs
LIB		= ft_atoi.c \
		  ft_isdigit.c \
		  ft_isspace.c \
		  ft_strchr.c \
		  ft_strlen.c \
		  ft_strtoll.c \
		  ft_stack.c \
		  ft_stack_operation.c \
		  ft_math.c \

LIBS	= $(addprefix $(LIB_DIR), $(LIB))

DEPS	= $(SRC:%.c:%.d) $(LIB:%.c:%.d)

OBJ_DIR	= ./objs
OBJS	= $(addprefix $(OBJ_DIR), $(SRC:%.c:%.o)) $(addprefix $(OBJ_DIR), $(LIB:%.c:%.o))

INCLUDE_DIR	= ./includes
IFLAGS	= $(addprefix -I, $(INCLUDE_DIR))


$(NAME)	: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@

$(OBJ_DIR)/%.o	:%.c
	@mkdir -p $$(dirname $@)
	$(CC) $(CFLAGS) $(IFLAGS) -c $< -o $@

all		: $(NAME)

claen	:
	rm -rf $(OBJ_DIR)


fclean	: clean
	rm -f $(NAME)

re		: fclaen all

test:
	make re WITH_SANITIZE=1

leaks	:
	make re WITH_LEAKS=1

norm	:
	@norminette -v
	@norminette $(SRC_DIR) $(LIB_DIR) $(INCLUDE_DIR)

.PHONY	: all clean fclean re test leaks norm

-include	$(DEPS)