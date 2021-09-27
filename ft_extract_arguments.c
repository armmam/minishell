/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_extract_arguments.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amamian <amamian@student.42yerevan.am>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/27 13:40:00 by amamian           #+#    #+#             */
/*   Updated: 2021/09/27 13:40:01 by amamian          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * returns 0 upon success, 1 upon failure
 *
 * the first condition checks for whether << is the last token in the command
 */
int	ft_parseheredoc(char ***token, char ***quote, t_cmd *cmd)
{
	char	*error;

	(*token)++;
	(*quote)++;
	if (*token == NULL || **token == NULL || ((*token)[0][0] == '|'
		&& (*token)[0][1] == '\0') || !ft_strcmp(**token, "<<"))
	{
		if (*token == NULL || **token == NULL)
			error = "newline";
		else
			error = *(*token - 1);
		ft_error(error, "syntax error");
		g_data.status = 258;
		return (1);
	}
	if (!cmd->heredoc)
		cmd->heredoc = ft_darrnew(0);
	ft_darrpushback(cmd->heredoc, ft_strdup(**token));
	if (!cmd->refine)
		cmd->refine = ft_darrnew(0);
	ft_darrpushback(cmd->refine, ft_strdup(**quote));
	return (0);
}

/*
 * returns 0 upon success, 1 upon failure
 *
 * closes `cmd_fd` if already encoutered </>>/> before for this command
 */
int	ft_parsefiletoken(char ***token, char ***quote, int *cmd_fd, int open_flag)
{
	int		fd;
	char	dir;

	dir = ***token;
	(*token)++;
	(*quote)++;
	if (ft_islastredir_tworedirs(token, quote))
	{
		ft_error(*(*token - 1), "syntax error");
		g_data.status = 258;
		return (1);
	}
	if ((dir == '>' && *cmd_fd != 1) || (dir == '<' && *cmd_fd != 0))
		close(*cmd_fd);
	fd = open(**token, open_flag, 0644);
	if (fd == -1)
	{
		perror("minishell");
		g_data.status = 1;
		return (1);
	}
	*cmd_fd = fd;
	return (0);
}

/*
 * haven't run out of tokens and haven't encountered a pipe, now need to
 * parse whatever got:
 *   if      <<
 *   else if >
 *   else if >>
 *   else if <
 *   else    an arg
 */
int	ft_parsevalidtoken(t_cmd *cmd, char ***token, char ***quote, t_darr	**args)
{
	int		err;

	err = 0;
	if (!ft_strcmp(**token, "<<"))
		err = ft_parseheredoc(token, quote, cmd);
	else if (!ft_strcmp(**token, ">"))
		err = ft_parsefiletoken(token, quote, &(cmd->out), O_TRUNC | O_WRONLY
				| O_CREAT);
	else if (!ft_strcmp(**token, ">>"))
		err = ft_parsefiletoken(token, quote, &(cmd->out), O_APPEND | O_WRONLY
				| O_CREAT);
	else if (!ft_strcmp(**token, "<"))
		err = ft_parsefiletoken(token, quote, &(cmd->in), O_RDONLY);
	else
		ft_darrpushback(*args, ft_strdup(**token));
	return (err);
}

/*
 * iterates over tokens while have not run out of tokens and have not
 * encountered a pipe
 */
int	ft_itertokens(t_cmd *cmd, char ***token, char ***quote, t_darr **args)
{
	int		err;

	err = 0;
	while (*token && **token && ft_strcmp(**token, "|"))
	{
		err = ft_parsevalidtoken(cmd, token, quote, args);
		if (err)
		{
			ft_darrclear(*args);
			return (1);
		}
		ft_next_tokenquote(token, quote);
	}
	return (0);
}

/*
 * puts char **args into given command's args field;
 * opens the files given as <, > or >> with open()
 * and writes them into the in and out fields (don't
 * forget that if multiple outfiles are received,
 * they have to be opened and closed except for the last
 * one. store only the last one)
 *  parses 'heredoc' if applicable; else assigns NULL to
 *  it -- it's null because of ft_calloc
 *
 * returns a pointer to the next command in `tokens`
*/
char	**ft_extract_arguments(t_cmd *cmd, char ***token, char ***quote)
{
	t_darr	*args;
	int		ret;

	args = ft_darrnew(0);
	ret = ft_itertokens(cmd, token, quote, &args);
	if (ret)
		return (NULL);
	ret = ft_process_pipes(token, quote, &args);
	if (ret)
		return (NULL);
	if (args->len > 0)
	{
		ft_darrpushback(args, NULL);
		cmd->args = args->ptr;
	}
	else
		free(args->ptr);
	free(args);
	return (*token);
}
