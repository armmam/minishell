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
	or,
	none
};

typedef struct s_cmd
{
	// index (to wait and abort properly)
	int		i;
	// args for execve
	char	**args;
	// last stdin, stdout for a command (open not the last ones with TRUNC if it's not >>; open with APPEND if >>)
	int		in;
	int		out;
	// preceding operator (or, and or none)
	int		cond;
	// heredoc (for <<). if absent, please make it NULL
	char	*heredoc;
}				t_cmd;

typedef struct s_env
{
	// number of commands in the current minishell session
	int		cmds;
	// how many additional processes have to be launched for the current minishell session
	int		prcs;
	// array of pids of launched processes
	pid_t	*family;
	// env vars of the current minishell session
	t_dmtx	*env;
	// status of the process that was the latest of terminate
	int		status;
}				t_env;

t_env	g_data;

void	ft_inheritenviron(char **environ);
void	ft_interpret(char *line);
char	*ft_getenv(const char *name);
void	ft_error(char *name, char *desc);
void	ft_exec(t_cmd *cmd);
int		ft_execbuiltin(t_cmd *cmd);
int		ft_convertbuiltin(char *builtin);
int		ft_echo(t_cmd *cmd);
int		ft_cd(t_cmd *cmd);
int		ft_pwd(t_cmd *cmd);
int		ft_export(t_cmd *cmd);
int		ft_unset(t_cmd *cmd);
int		ft_env(t_cmd *cmd);
void	ft_abort(t_cmd *cmd);
char	**ft_tokenize(const char *line);
int		ft_isbuiltin(char *builtin);

// remove these after done with testing
int		ft_extracttoken(const char *line, char **token);

#endif