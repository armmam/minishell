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

enum	e_builtins
{
	__echo = 1,
	__cd,
	__pwd,
	__export,
	__unset,
	__env,
	__exit	
};

enum	e_operators
{
	and,
	or
};

typedef struct s_cmd
{
	// index (to wait and abort properly)
	int		i;
	// args for execve
	char	**args;
	// pids of the children
	pid_t	*family;
	// stdins, stdouts, stderrs for a command
	int		*in;
	int		*out;
	int		*err;
	// preceding operator
	int		cond;
}				t_cmd;

typedef struct s_env
{
	int		cmdcount;
	pid_t	*family;
	char	**env;
	int		status;
	t_cmd	cmd;
}				t_env;

t_env	g_data;

void	ft_inheritenviron(char **environ);
void	ft_interpret(char *line);
char	*ft_getenv(const char *name);
int		ft_error(char *name, char *desc);
void	ft_exec(t_cmd *cmd);
int		ft_execbuiltin(t_cmd *cmd);
int		ft_convertbuiltin(char *builtin);
int		ft_echo(char **args);
int		ft_cd(char **args);
int		ft_pwd(char **args);
int		ft_export(char **args);
int		ft_unset(char **args);
int		ft_env(char **args);
void	ft_extractinfiles(t_cmd *cmd, char **tokens);
void	ft_extractoutfiles(t_cmd *cmd, char **tokens);
void	ft_abort(t_cmd *cmd);

#endif