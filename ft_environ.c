#include "minishell.h"

void	ft_inherit_environment(char **environ)
{
	int	i;

	g_data.env = ft_darrnew(ft_matrixlen(environ) + 1);
	i = 0;
	while (environ[i])
	{
		ft_darrpushback(g_data.env, ft_strdup(environ[i]));
		i++;
	}
	g_data.env->ptr[i] = NULL;
}

char	*ft_getenv_full(const char *name)
{
	int		i;
	int		name_len;

	i = 0;
	if (!ft_isvalididentifier(name))
		return (NULL);
	while (g_data.env->ptr[i])
	{
		name_len = ft_strlen((char *) name);
		if (!ft_strncmp(g_data.env->ptr[i], name, name_len) && (g_data.env->ptr[i][name_len] == '=' || g_data.env->ptr[i][name_len] == '\0'))
			return (g_data.env->ptr[i]);
		i++;
	}
	return (NULL);
}

char	*ft_getenv(const char *name)
{
	char	*env;

	env = ft_getenv_full(name);
	if (env)
		return (env + ft_strlen((char *) name) + 1);
	return (NULL);
}