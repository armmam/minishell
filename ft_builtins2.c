#include "minishell.h"

int	ft_env(char **args)
{
	int	i;
	int	j;

	(void)args;
	i = 0;
	while (g_data.env[i])
	{
		j = 0;
		while (g_data.env[i][j] != '=' && g_data.env[i][j + 1] != '\0')
			j++;
		j++;
		if (g_data.env[i][j])
		{
			ft_putstr_fd(g_data.env[i], 1);
			ft_putstr_fd("\n", 1);
		}
		i++;
	}
	return (1);
}
