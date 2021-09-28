/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_receive_heredoc.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amamian <amamian@student.42yerevan.am>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/27 17:15:19 by amamian           #+#    #+#             */
/*   Updated: 2021/09/27 17:15:20 by amamian          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_read_heredoc_input(t_cmd *cmd, t_heredocr *h, int write_ends_k)
{
	char	*temp;

	temp = readline("> ");
	if (temp == NULL)
		return (1);
	else if (!ft_strcmp(temp, h->heredoc))
	{
		free(temp);
		if (h->j == cmd->heredoc->len)
			close(write_ends_k);
		return (1);
	}
	if (h->refined)
		temp = ft_refineline(temp);
	if (h->j == cmd->heredoc->len)
	{
		ft_putstr_fd(temp, write_ends_k);
		ft_putstr_fd("\n", write_ends_k);
	}
	free(temp);
	return (0);
}

void	ft_receive_heredoc(t_cmd *cmd, int k, int *write_ends)
{
	t_heredocr	h;
	int			br;

	if (cmd->heredoc)
	{
		h.j = 0;
		while (h.j < cmd->heredoc->len)
		{
			h.refined = 1;
			if (!ft_strcmp(cmd->refine->ptr[h.j], "\'")
				&& h.j == cmd->heredoc->len - 1)
				h.refined = 0;
			h.heredoc = cmd->heredoc->ptr[h.j];
			h.j++;
			while (1)
			{
				br = ft_read_heredoc_input(cmd, &h, write_ends[k]);
				if (br)
					break ;
			}
		}
	}
}
