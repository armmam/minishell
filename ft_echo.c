/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amamian <amamian@student.42yerevan.am>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/27 17:48:12 by amamian           #+#    #+#             */
/*   Updated: 2021/09/28 11:17:29 by amamian          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * returns 1 if `arg` is "-n" or "-nn" or a '-' and any number of 'n's
 * otherwise returns 0
 */
int	ft_isoptionn(char **arg)
{
	int	i;

	if (*arg && !ft_strncmp(*arg, "-n", 2))
	{
		i = 2;
		while ((*arg)[i])
		{
			if ((*arg)[i++] != 'n')
				return (0);
		}
		return (1);
	}
	return (0);
}

void	ft_processoptionn(char ***arg, int *nl)
{
	while (ft_isoptionn(*arg))
	{
		*nl = 0;
		(*arg)++;
	}
}

int	ft_echo(t_cmd *cmd)
{
	char	**arg;
	int		nl;

	arg = (cmd->args) + 1;
	nl = 1;
	ft_processoptionn(&arg, &nl);
	if (!*arg)
	{
		if (nl)
			ft_putstr_fd("\n", cmd->out);
		return (0);
	}
	while (*arg)
	{
		ft_putstr_fd(*arg, cmd->out);
		arg++;
		if (*arg)
			ft_putstr_fd(" ", cmd->out);
	}
	if (nl)
		ft_putstr_fd("\n", cmd->out);
	return (0);
}
