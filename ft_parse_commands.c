/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parse_commands.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amamian <amamian@student.42yerevan.am>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/27 13:40:07 by amamian           #+#    #+#             */
/*   Updated: 2021/09/27 13:40:08 by amamian          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * initializes a pipe and fills `commands` accordingly
 */
void	ft_init_pipe(t_cmd *commands, int i)
{
	int		pipefd[2];

	pipe(pipefd);
	if (commands[i - 1].out != 1)
		close(pipefd[1]);
	else
		commands[i - 1].out = pipefd[1];
	if (commands[i].in != 0)
		close(pipefd[0]);
	else
		commands[i].in = pipefd[0];
}

/*
 * initializes `in`s and `out`s in `commands` for each command accordingly
 */
int	ft_init_inoutstreams(t_cmd *commands, char ***token, char ***quote, int i)
{
	while (i < g_data.cmds)
	{
		commands[i].in = 0;
		commands[i].out = 1;
		commands[i].i = i;
		if (!(ft_extract_arguments(&commands[i], token, quote)))
		{
			ft_free_commands(commands);
			return (1);
		}
		if (i)
			ft_init_pipe(commands, i);
		i++;
	}
	return (0);
}

t_cmd	*ft_parse_commands(t_tokens *tokens)
{
	int		i;
	t_cmd	*commands;
	char	**token;
	char	**quote;
	int		ret;

	if (!tokens || !tokens->tokens || !tokens->tokens->ptr
		|| !*tokens->tokens->ptr)
		return (NULL);
	token = tokens->tokens->ptr;
	quote = tokens->quotes->ptr;
	commands = ft_calloc(g_data.cmds, sizeof(t_cmd));
	i = 0;
	if (!ft_strcmp(token[i], "|"))
	{
		ft_error(token[i], "syntax error");
		g_data.status = 258;
		free(commands);
		return (NULL);
	}
	ret = ft_init_inoutstreams(commands, &token, &quote, i);
	if (ret)
		return (NULL);
	return (commands);
}
