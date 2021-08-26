#include "minishell.h"

void	ft_inherit_environment(char **environ)
{
	int	i;

	g_data.env = ft_darrnew(ft_matrixlen(environ) + 1);
	i = 0;
	while (environ[i])
	{
		if (ft_strncmp(environ[i], "OLDPWD=", ft_strlen("OLDPWD=")))
			ft_darrpushback(g_data.env, ft_strdup(environ[i]));
		i++;
	}
}

char	*ft_getenv_full(const char *name)
{
	int		i;
	int		name_len;
	char	*identifier;

	i = 0;
	identifier = ft_separate_identifier((char *) name);
	if (!ft_isvalididentifier(identifier))
	{
		free(identifier);
		return (NULL);
	}
	while (g_data.env->ptr[i])
	{
		name_len = ft_strlen(identifier);
		if (!ft_strncmp(g_data.env->ptr[i], identifier, name_len) && (g_data.env->ptr[i][name_len] == '=' || g_data.env->ptr[i][name_len] == '\0'))
		{
			free(identifier);
			return (g_data.env->ptr[i]);
		}
		i++;
	}
	free(identifier);
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