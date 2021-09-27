/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_utils1.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aisraely <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/27 17:24:52 by aisraely          #+#    #+#             */
/*   Updated: 2021/09/27 17:24:53 by aisraely         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_count_commands(char **tokens)
{
	int	i;
	int	count;

	if (!tokens || !(*tokens))
		return (0);
	count = 1;
	i = 0;
	while (tokens[i])
	{
		if (tokens[i][0] == '|' && tokens[i][1] == '\0')
			count++;
		i++;
	}
	return (count);
}

static void	ft_free_family(void)
{
	if (g_data.family)
	{
		free(g_data.family);
		g_data.family = NULL;
	}
}

/*
 * frees commands 
 */
void	ft_free_commands(t_cmd *cmds)
{
	int	i;

	i = 0;
	if (cmds)
	{
		while (i < g_data.cmds)
		{
			if (cmds[i].in != 0 && cmds[i].in != 1)
				close(cmds[i].in);
			if (cmds[i].out != 1 && cmds[i].out != 0)
				close(cmds[i].out);
			if (cmds[i].heredoc)
				ft_darrclear(cmds[i].heredoc);
			if (cmds[i].refine)
				ft_darrclear(cmds[i].refine);
			if (cmds[i].args)
				ft_freematrix(cmds[i].args);
			i++;
		}
		free(cmds);
		cmds = NULL;
	}
	ft_free_family();
}

void	ft_free_tokens(t_tokens *tokens)
{
	if (tokens)
	{
		if (tokens->tokens)
			ft_darrclear(tokens->tokens);
		if (tokens->quotes)
			ft_darrclear(tokens->quotes);
		free(tokens);
	}
}

t_cmd	*ft_find_command(pid_t pid)
{
	int	i;

	i = 0;
	while (i < g_data.cmds)
	{
		if (g_data.family[i] == pid)
			return (&g_data.commands[i]);
		i++;
	}
	return (NULL);
}
