/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_execute.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aisraely <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/27 16:52:45 by aisraely          #+#    #+#             */
/*   Updated: 2021/09/27 16:52:46 by aisraely         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_execbuiltin(t_cmd *cmd)
{
	int	ret;

	ret = ft_convert_builtin(cmd->args[0]);
	if (ret == __echo)
		g_data.status = ft_echo(cmd);
	else if (ret == __cd)
		g_data.status = ft_cd(cmd);
	else if (ret == __pwd)
		g_data.status = ft_pwd(cmd);
	else if (ret == __export)
		g_data.status = ft_export(cmd);
	else if (ret == __unset)
		g_data.status = ft_unset(cmd);
	else if (ret == __env)
		g_data.status = ft_env(cmd);
	else if (ret == __exit)
		ft_exit(cmd);
	if (ret)
		return (0);
	return (-1);
}

void	ft_close_descriptors(void)
{
	int	i;

	i = 0;
	while (i < g_data.cmds)
	{
		if (g_data.commands[i].in != 0)
			close(g_data.commands[i].in);
		if (g_data.commands[i].out != 1)
			close(g_data.commands[i].out);
		i++;
	}
}

static void	ft_traverse_binaries(t_cmd *cmd)
{
	int		i;
	char	*newpath;
	char	**paths;

	if (ft_execbuiltin(cmd) == -1)
	{
		execve(cmd->args[0], cmd->args, g_data.env->ptr);
		if (ft_getenv("PATH"))
		{
			paths = ft_split(ft_getenv("PATH"), ':');
			i = 0;
			while (paths[i])
			{
				newpath = ft_strjoin3(paths[i], "/", cmd->args[0]);
				execve(newpath, cmd->args, g_data.env->ptr);
				free(newpath);
				i++;
			}
			ft_freematrix(paths);
			ft_error(cmd->args[0], "command not found");
		}
		else
			ft_error(cmd->args[0], "No such file or directory");
	}
}

void	ft_exec(t_cmd *cmd)
{
	if (!ft_isbuiltin(cmd->args[0]) || g_data.cmds != 1)
	{
		if (dup2(cmd->in, 0) == -1 || dup2(cmd->out, 1) == -1)
		{
			perror("minishell");
			exit(1);
		}
		ft_close_descriptors();
		cmd->out = 1;
	}
	if (cmd->args[0][0] == '/')
	{
		execve(cmd->args[0], cmd->args, g_data.env->ptr);
		ft_error(cmd->args[0], "No such file or directory");
	}
	else
		ft_traverse_binaries(cmd);
	if (!(g_data.cmds == 1 && ft_isbuiltin(cmd->args[0])))
	{
		if (ft_isbuiltin(cmd->args[0]))
			exit(g_data.status);
		exit(127);
	}
}
