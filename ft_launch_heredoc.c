/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_launch_heredoc.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amamian <amamian@student.42yerevan.am>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/27 17:15:25 by amamian           #+#    #+#             */
/*   Updated: 2021/09/27 17:15:26 by amamian          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_save_write_end(int **write_ends, int i, int *j)
{
	int	pipefd[2];

	if (g_data.commands[i].heredoc)
	{
		pipe(pipefd);
		if (g_data.commands[i].in != 0)
			close(g_data.commands[i].in);
		g_data.commands[i].in = pipefd[0];
		(*write_ends)[(*j)++] = pipefd[1];
	}
}

/*
 * returns the number of heredocs, saves the write-ends
 * of the heredoc
 */
int	ft_save_write_ends(int **write_ends)
{
	int	i;
	int	j;
	int	count;

	count = 0;
	i = 0;
	while (i < g_data.cmds)
		if (g_data.commands[i++].heredoc)
			count++;
	*write_ends = malloc(count * sizeof(int *));
	i = 0;
	j = 0;
	while (i < g_data.cmds)
	{
		ft_save_write_end(write_ends, i, &j);
		i++;
	}
	return (count);
}

/*
 * is run inside of the child process and receives all heredocs
 */
void	ft_process_heredocs_childp(t_heredocl *h)
{
	ft_heredoc_signals();
	h->i = 0;
	while (h->i < g_data.cmds)
	{
		if (g_data.commands[h->i].heredoc)
			ft_receive_heredoc(&g_data.commands[h->i], h->heredocs++,
				h->write_ends);
		h->i++;
	}
	exit(0);
}

/*
 * initializes the values for ft_lauch_heredoc
 */
int	ft_init_launch_heredoc(t_heredocl *h)
{
	h->write_ends = NULL;
	h->heredocs = ft_save_write_ends(&(h->write_ends));
	if (!h->write_ends)
		return (1);
	ft_ignore_signals();
	h->heredoc_pid = fork();
	h->i = 0;
	while (h->heredoc_pid && h->i < h->heredocs)
		close(h->write_ends[h->i++]);
	h->heredocs = 0;
	return (0);
}

/*
 * launches a process for receiveng heredoc;
 * if it was terminated by a SIGINT signal, returns 1;
 * else if exits successfully, returns 0 
 */
int	ft_launch_heredoc(void)
{
	t_heredocl	h;
	int			ret;

	ret = ft_init_launch_heredoc(&h);
	if (ret)
		return (0);
	if (h.heredoc_pid == 0)
		ft_process_heredocs_childp(&h);
	wait(&(h.heredoc_status));
	free(h.write_ends);
	ft_define_signals();
	if (WEXITSTATUS(h.heredoc_status) == 1)
	{
		g_data.status = 1;
		ft_putstr_fd("\n", 1);
		return (1);
	}
	return (0);
}
