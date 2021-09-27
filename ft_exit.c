/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aisraely <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/27 17:23:20 by aisraely          #+#    #+#             */
/*   Updated: 2021/09/27 17:23:22 by aisraely         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	ft_terminate_process(int exit_code)
{
	if (g_data.cmds == 1)
	{
		if (!g_data.status)
			exit(exit_code);
	}
	else
	{
		if (!exit_code)
			exit(g_data.status);
		exit(exit_code);
	}
}

void	ft_exit(t_cmd *cmd)
{
	int	exit_code;

	g_data.status = 0;
	exit_code = 0;
	if (g_data.cmds == 1)
		ft_putstr_fd("exit\n", 2);
	if (ft_matrixlen(cmd->args) == 2)
	{
		if (!ft_isdigitstr(cmd->args[1]))
			g_data.status = ft_error("exit", "numeric argument required");
		else
			exit_code = ft_atoi(cmd->args[1]);
	}
	else if (ft_matrixlen(cmd->args) > 2)
		g_data.status = ft_error("exit", "too many arguments");
	ft_terminate_process(exit_code);
}
