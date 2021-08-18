#include "minishell.h"

void	ft_inheritenviron(char **environ)
{
	int	i;

	g_data.env = ft_dmtxnew(ft_matrixlen(environ) + 1);
	i = 0;
	while (environ[i])
	{
		g_data.env->ptr[i] = ft_strdup(environ[i]);
		i++;
	}
	g_data.env->ptr[i] = NULL;
}

char	*ft_getenv(const char *name)
{
	int		i;
	char	*var;

	i = 0;
	if (ft_strrchr(name, '='))
		return (NULL);
	while (g_data.env->ptr[i])
	{
		if (!ft_strncmp(g_data.env->ptr[i], name, ft_strlen((char *) name))) // another likely typo
		{
			var = g_data.env->ptr[i];
			var += ft_strlen((char *) name) + 1;
			return (var);
		}
		i++;
	}
	return (NULL);
}