#include "minishell.h"

int	ft_env(t_cmd *cmd)
{
	int	i;
	int	j;

	i = 0;
	while (g_data.env->ptr[i])
	{
		j = 0;
		while (g_data.env->ptr[i][j] && g_data.env->ptr[i][j] != '=')
			j++;
		if (g_data.env->ptr[i][j])
		{
			ft_putstr_fd(g_data.env->ptr[i], cmd->out);
			ft_putstr_fd("\n", cmd->out);
		}
		i++;
	}
	return (0);
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