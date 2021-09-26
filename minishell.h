/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amamian <amamian@student.42yerevan.am>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/26 17:41:36 by amamian           #+#    #+#             */
/*   Updated: 2021/09/26 17:41:37 by amamian          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "./libft/libft.h"

# define _SVID_SOURCE 1

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

typedef struct s_cmd
{
	int		i;
	char	**args;
	int		in;
	int		out;
	t_darr	*heredoc;
	t_darr	*refine;
}				t_cmd;

typedef struct s_env
{
	int		cmds;
	t_cmd	*commands;
	pid_t	*family;
	t_darr	*env;
	int		status;
}				t_env;

typedef struct s_tokens
{
	t_darr	*tokens;
	t_darr	*quotes;
}				t_tokens;

t_env	g_data;

void		ft_inherit_environment(char **environ);
void		ft_interpret(char *line);
char		*ft_getenv(const char *name);
int			ft_error(char *name, char *desc);
void		ft_exec(t_cmd *cmd);
int			ft_execbuiltin(t_cmd *cmd);
int			ft_convertbuiltin(char *builtin);
int			ft_echo(t_cmd *cmd);
int			ft_cd(t_cmd *cmd);
int			ft_pwd(t_cmd *cmd);
int			ft_export(t_cmd *cmd);
int			ft_unset(t_cmd *cmd);
int			ft_env(t_cmd *cmd);
void		ft_exit(t_cmd *cmd);
t_tokens	*ft_tokenize(const char *line);
int			ft_isbuiltin(char *builtin);
char		*ft_refineline(char *line);
t_cmd		*ft_parse_commands(t_tokens *tokens);
void		ft_free_commands(t_cmd *cmds);
int			ft_isquoted(char *str, char c);
int			ft_isvalididentifier(const char *variable);
int			ft_isvaliddeclaration(char *decl);
char		*ft_separate_identifier(char *decl);
char		*ft_getenv_full(const char *name);
void		ft_define_signals(void);
void		ft_default_signals(void);
void		ft_heredoc_signals(void);
void		ft_ignore_signals(void);
void		ft_reprompt(int sig);
void		ft_do_nothing(int sig);
void		ft_suppress_output(void);
void		ft_get_interrupted(int sig);
int			ft_launch_heredoc(void);
void		ft_receive_heredoc(t_cmd *cmd, int j, int *write_ends);

#endif