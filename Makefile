NAME		= minishell

USER		= $(shell whoami)

SRCS		= $(wildcard *.c)
OBJS		= $(SRCS:.c=.o)
LIBFT		= libft/libft.a
CC			= gcc -Wall -Werror -Wextra -g
RM			= rm -rf

# ATTENTION! These flags are machine-specific. Edit them so that they point
# to the location of your `readline` library.
ifeq ($(USER), aisraely)
	LDFLAGS		= -L/Users/aisraely/.brew/opt/readline/lib
	CPPFLAGS	= -I/Users/aisraely/.brew/opt/readline/include
else ifeq ($(USER), arman)
	LDFLAGS		= -L/Users/arman/.brew/opt/readline/lib
	CPPFLAGS	= -I/Users/arman/.brew/opt/readline/include
else ifeq ($(USER), amamian)
	LDFLAGS		= -L/Users/amamian/.brew/opt/readline/lib
	CPPFLAGS	= -I/Users/amamian/.brew/opt/readline/include
else
endif

all: $(NAME)

$(NAME): $(OBJS) $(LIBFT)
	@$(CC) $(OBJS) -o $(NAME) $(LIBFT) -lreadline $(LDFLAGS) $(CPPFLAGS)

$(LIBFT):
	@make all -C ./libft --silent

clean:
	@$(RM) $(OBJS)
	@make clean -C ./libft --silent

norme:
	@norminette -R CheckForbiddenSourceHeader *.c *.h libft/*.c libft/*.h

fclean: clean
	@$(RM) $(NAME)
	@make fclean -C ./libft --silent

re:			fclean all

.PHONY:		all clean fclean re