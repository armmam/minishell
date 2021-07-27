#ifndef MINISHELL_H
# define MINISHELL_H

# include "./libft/libft.h"

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>

# include <readline/readline.h>
# include <readline/history.h>

# include <signal.h>

# include <sys/stat.h>
# include <sys/errno.h>
# include <sys/ioctl.h>

# include <dirent.h>

# include <string.h>

# include <termios.h>

# include <curses.h>
# include <term.h>

typedef struct s_env
{
	char	**env;
	int		status;
}				t_env;

t_env	g_data;

void	ft_inheritenviron(char **environ);
void	ft_interpret(char *line);
char	*ft_getenv(const char *name);
int		ft_error(char *name, char *desc);
void	ft_exec(char **args, char **paths);
int		ft_execbuiltin(char **args);

#endif