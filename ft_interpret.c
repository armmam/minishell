/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_interpret.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aisraely <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/27 16:50:11 by aisraely          #+#    #+#             */
/*   Updated: 2021/09/27 16:50:14 by aisraely         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	ft_update_status(int terminated, int returned)
{
	int		signaled;
	t_cmd	*selected;

	signaled = 0;
	if (terminated == g_data.family[g_data.cmds - 1])
	{
		if (!WTERMSIG(returned))
			g_data.status = WEXITSTATUS(returned);
		else
		{
			signaled = 1;
			g_data.status = WTERMSIG(returned) + 128;
		}
	}
	selected = ft_find_command(terminated);
	if (selected)
	{
		if (selected->out != 1)
			close(selected->out);
		if (selected->in != 0)
			close(selected->in);
	}
	return (signaled);
}

static void	ft_block_main_process(void)
{
	int		i;
	int		returned;
	int		signaled;

	signaled = 0;
	i = 0;
	while (i < g_data.cmds)
	{
		signaled += ft_update_status(wait(&returned), returned);
		i++;
	}
	if (signaled)
	{
		if (g_data.status == 131)
			ft_putstr_fd("Quit: 3", 1);
		ft_putstr_fd("\n", 1);
	}
}

static void	ft_fork_processes(void)
{
	int	i;

	if (g_data.cmds == 1 && g_data.commands[0].args
		&& ft_isbuiltin(g_data.commands[0].args[0]))
		ft_exec(&g_data.commands[0]);
	else
	{
		i = 0;
		while (i < g_data.cmds)
		{
			g_data.family[i] = fork();
			if (g_data.family[i] == 0)
			{
				if (!g_data.commands[i].args)
					exit(0);
				ft_default_signals();
				ft_exec(&g_data.commands[i]);
			}
			i++;
		}
		ft_ignore_signals();
		ft_block_main_process();
		ft_define_signals();
	}
}

void	ft_interpret(char *line)
{
	t_tokens	*tokens;

	tokens = ft_tokenize(line);
	if (!tokens->tokens)
		return ;
	g_data.cmds = ft_count_commands(tokens->tokens->ptr);
	g_data.commands = ft_parse_commands(tokens);
	if (!g_data.commands || ft_launch_heredoc())
	{
		ft_free_commands(g_data.commands);
		ft_free_tokens(tokens);
		return ;
	}
	g_data.family = ft_calloc(g_data.cmds, sizeof(pid_t));
	ft_fork_processes();
	ft_free_commands(g_data.commands);
	ft_free_tokens(tokens);
}
