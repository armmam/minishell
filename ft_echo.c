/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amamian <amamian@student.42yerevan.am>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/27 17:48:12 by amamian           #+#    #+#             */
/*   Updated: 2021/09/27 17:48:13 by amamian          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_echo(t_cmd *cmd)
{
	char	**arg;
	int		nl;

	arg = (cmd->args) + 1;
	nl = 1;
	if (*arg && !ft_strcmp(*arg, "-n"))
	{
		nl = 0;
		arg++;
	}
	if (!*arg)
	{
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
