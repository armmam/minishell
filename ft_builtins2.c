#include "minishell.h"

int	ft_env(t_cmd *cmd)
{
	int	i;
	int	j;

	i = 0;
	while (g_data.env->ptr[i])
	{
		j = 0;
		while (g_data.env->ptr[i][j] != '=' && g_data.env->ptr[i][j + 1] != '\0')
			j++;
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
