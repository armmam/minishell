#include "minishell.h"

int	ft_env(char **args)
{
	int	i;

	(void)args;
	i = 0;
	while (g_data.env[i])
	{
		ft_putstr_fd(g_data.env[i], 1);
		ft_putstr_fd("\n", 1);
		i++;
	}
	return (1);
}

// Alex: gotta find a way to force the parent process quit instead of 
// the forked child. maybe convert the builtin before forking and in case
// of exit just call exit? 
int	ft_exit(char **args)
{
	(void)args;
	// exit(0);
	return (1);
}