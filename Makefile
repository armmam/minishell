NAME		= minishell

SRCS		= $(wildcard *.c)
OBJS		= ${SRCS:.c=.o}
CC			= gcc
CFLAGS		= -Wall -Wextra -Werror
RM			= rm -rf

# ATTENTION! These flags are machine-specific. Edit them so that they point
# to the location of your `readline` library. Right now, it is assumed that
# your homebrew is installed at /iSCSI.
ifeq ($$(whoami), aisraely)
	LDFLAGS		= -L/iSCSI/.brew/opt/readline/lib
	CPPFLAGS	= -I/iSCSI/.brew/opt/readline/include
else
	LDFLAGS		= 
	CPPFLAGS	= 
endif

all: $(NAME)

${NAME}: ${OBJS}
	@echo [minishell] Compiling Libft...
	@${MAKE} -C ./libft --silent
	@echo [minishell] Compiling minishell...
	@${CC} ${OBJS} ${CFLAGS} ${LDFLAGS} ${CPPFLAGS} -lreadline ./libft/libft.a -o ${NAME}
	@echo [minishell] minishell successfully compiled.

clean:
	@${MAKE} -C libft fclean --silent
	@${RM} ${OBJS}

fclean: clean
	@${RM} ${NAME}

norme:
	@norminette -R CheckForbiddenSourceHeader *.c *.h libft/*.c libft/*.h
	
bonus: all

re: fclean all

.PHONY: clean fclean re all bonus
