NAME		= minishell

SRCS		= $(wildcard *.c)
OBJS		= $(SRCS:.c=.o)
LIBFT		= libft/libft.a
CC			= gcc
FLAGS		= -Wall -Werror -Wextra
RM			= rm -rf

# ATTENTION! These flags are machine-specific. Edit them so that they point
# to the location of your `readline` library.
ifeq ($$(whoami), arman)
	LDFLAGS		= -L~/.brew/opt/readline/lib
	CPPFLAGS	= -I~/.brew/opt/readline/include
else
	LDFLAGS		= -L/iSCSI/.brew/opt/readline/lib
	CPPFLAGS	= -I/iSCSI/.brew/opt/readline/include
endif

all: $(NAME)

$(NAME): $(OBJS) $(LIBFT)
	@$(CC) $(FLAGS) $(OBJS) -o $(NAME) $(LIBFT) -lreadline $(LDFLAGS) $(CPPFLAGS)

$(LIBFT):
	@make all -C ./libft --silent

clean:
	@$(RM) $(OBJS)
	@make clean -C ./libft --silent

fclean: clean
	@$(RM) $(NAME)
	@make fclean -C ./libft --silent

re:			fclean all

.PHONY:		all clean fclean re
