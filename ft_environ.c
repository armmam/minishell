#include "minishell.h"

void	ft_inheritenviron(char **environ)
{
	int	i;

	g_data.env = ft_darrnew(ft_matrixlen(environ) + 1);
	i = 0;
	while (environ[i])
	{
		g_data.env->ptr[i] = ft_strdup(environ[i]);
		i++;
	}
	g_data.env->ptr[i] = NULL;
}

char	*ft_getenv_full(const char *name)
{
	int		i;

	i = 0;
	if (!ft_isvalididentifier(name))
		return (NULL);
	while (g_data.env->ptr[i])
	{
		if (!ft_strncmp(g_data.env->ptr[i], name, ft_strlen((char *) name)))
			return (g_data.env->ptr[i]);
		i++;
	}
	return (NULL);
}

char	*ft_getenv(const char *name)
{
	return (ft_getenv_full(name) + ft_strlen((char *) name) + 1);
}