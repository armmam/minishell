/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amamian <amamian@student.42yerevan.am>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/26 17:41:36 by amamian           #+#    #+#             */
/*   Updated: 2021/09/27 17:37:53 by amamian          ###   ########.fr       */
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

typedef struct s_refine
{
	char		*ptr;
	char		*prefix;
	char		*postfix;
	char		*val;
}				t_refine;

typedef struct s_expandt
{
	size_t		i;
	size_t		j;
	size_t		expand;
}				t_expandt;

typedef struct s_heredocl
{
	int			i;
	int			heredocs;
	int			*write_ends;
	int			heredoc_pid;
	int			heredoc_status;
}				t_heredocl;

typedef struct s_heredocr
{
	int			refined;
	char		*heredoc;
	size_t		j;
}				t_heredocr;

t_env	g_data;

void		ft_inherit_environment(char **environ);
void		ft_interpret(char *line);
char		*ft_getenv(const char *name);
int			ft_error(char *name, char *desc);
void		ft_exec(t_cmd *cmd);
int			ft_execbuiltin(t_cmd *cmd);
int			ft_convert_builtin(char *builtin);
int			ft_echo(t_cmd *cmd);
int			ft_cd(t_cmd *cmd);
int			ft_pwd(t_cmd *cmd);
int			ft_export(t_cmd *cmd);
int			ft_unset(t_cmd *cmd);
int			ft_env(t_cmd *cmd);
void		ft_exit(t_cmd *cmd);
int			ft_isbuiltin(char *builtin);
char		*ft_refineline(char *line);
t_cmd		*ft_find_command(pid_t pid);
void		ft_free_commands(t_cmd *cmds);
void		ft_free_tokens(t_tokens *tokens);
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
int			ft_count_commands(char **tokens);
void		ft_print_environment(int fd);
char		*ft_isdefined(char *decl);

/*
 * everything related to command parsing
 */
t_cmd		*ft_parse_commands(t_tokens *tokens);
int			ft_init_inoutstreams(t_cmd *commands, char ***token, char ***quote,
				int i);
void		ft_init_pipe(t_cmd *commands, int i);
char		**ft_extract_arguments(t_cmd *cmd, char ***token, char ***quote);
int			ft_process_pipes(char ***token, char ***quote, t_darr **args);
int			ft_itertokens(t_cmd *cmd, char ***token, char ***quote,
				t_darr **args);
int			ft_currpipe_nextpipenull(char ***token);
int			ft_currpipe_nextnot(char ***token);
void		ft_next_tokenquote(char ***token, char ***quote);
int			ft_parsevalidtoken(t_cmd *cmd, char ***token, char ***quote,
				t_darr	**args);
int			ft_parsefiletoken(char ***token, char ***quote, int *cmd_fd,
				int open_flag);
int			ft_islastredir_tworedirs(char ***token, char ***quote);
int			ft_parseheredoc(char ***token, char ***quote, t_cmd *cmd);

/*
 * everything related to tokenization
 */
t_tokens	*ft_tokenize(const char *line);
void		ft_darrpushback_tokens(t_tokens *ret, char *token, char *quote);
int			ft_extract_token(const char *line, char **token, char **quote);
void		ft_extract_token_init(const char *line, char **token, char **quote,
				t_expandt *e);
int			ft_process_tokenchar(const char *line, char **token, char **quote,
				t_expandt *e);
int			ft_process_tokenquote(const char *line, char **token, char **quote,
				t_expandt *e);
int			ft_extract_redirpipe(const char *line, char **token, t_expandt *e);
void		ft_appendtoken(char **token, const char *new, size_t len,
				int expand);
char		*ft_refineline(char *line);
int			ft_process_dollarsign(t_refine *r, char *line);
int			ft_envlen(char *line);

/*
 * everything related to heredocs
 */
int			ft_launch_heredoc(void);
int			ft_init_launch_heredoc(t_heredocl *h);
void		ft_process_heredocs_childp(t_heredocl *h);
int			ft_save_write_ends(int **write_ends);
void		ft_save_write_end(int **write_ends, int i, int *j);
void		ft_receive_heredoc(t_cmd *cmd, int k, int *write_ends);
int			ft_read_heredoc_input(t_cmd *cmd, t_heredocr *h, int write_ends_k);

#endif