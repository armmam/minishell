/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aisraely <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/27 17:22:13 by aisraely          #+#    #+#             */
/*   Updated: 2021/09/27 18:38:12 by aisraely         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	ft_refresh_paths(char *oldpwd)
{
	t_cmd	export;
	char	*pwd;

	pwd = getcwd(NULL, 0);
	export.args = ft_calloc(3, sizeof(char *));
	export.args[0] = ft_strdup("");
	export.args[1] = ft_strjoin("PWD=", pwd);
	ft_export(&export);
	ft_freematrix(export.args);
	free(pwd);
	export.args = ft_calloc(3, sizeof(char *));
	export.args[0] = ft_strdup("");
	export.args[1] = ft_strjoin("OLDPWD=", oldpwd);
	ft_export(&export);
	ft_freematrix(export.args);
}

static int	ft_change_path(t_cmd *cmd, int *status, char *oldpwd)
{
	if (cmd->args[1][0] == '-' && cmd->args[1][1] == '\0')
	{
		if (ft_getenv("OLDPWD"))
		{
			*status = ft_abs(chdir(ft_getenv("OLDPWD")));
			if (!*status)
			{
				ft_putstr_fd(ft_getenv("OLDPWD"), cmd->out);
				ft_putstr_fd("\n", cmd->out);
			}
		}
		else
		{
			free(oldpwd);
			return (ft_error("cd", "OLDPWD not set"));
		}
	}
	else
		*status = ft_abs(chdir(cmd->args[1]));
	return (0);
}

int	ft_cd(t_cmd *cmd)
{
	int		status;
	char	*oldpwd;

	status = 1;
	oldpwd = getcwd(NULL, 0);
	if (ft_matrixlen(cmd->args) == 1)
	{
		if (ft_getenv("HOME"))
			status = ft_abs(chdir(ft_getenv("HOME")));
		else
		{
			free(oldpwd);
			return (ft_error("cd", "HOME not set"));
		}
	}
	else
		if (ft_change_path(cmd, &status, oldpwd))
			return (1);
	if (!status)
		ft_refresh_paths(oldpwd);
	else
		status = ft_error("cd", "No such file or directory");
	free(oldpwd);
	return (status);
}
