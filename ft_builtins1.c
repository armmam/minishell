#include "minishell.h"

int	ft_echo(char **args)
{
	//
	(void)args;
	return (1);
}

void	ft_refreshpwds(char *oldpwd)
{
	char	exportargs[2];
	char	*pwd;

	getcwd(pwd, NULL);
	exportargs[1] = ft_strjoin("PWD=", pwd);
	ft_export(exportargs);
	exportargs[1] = ft_strjoin("OLDPWD=", oldpwd);
	ft_export(exportargs);
}

int	ft_cd(char **args)
{
	int		status;
	char	*oldpwd;

	status = 1;
	getcwd(oldpwd, NULL);
	if (ft_matrixlen(args) == 1)
	{
		if (ft_getenv("HOME"))
			status = chdir(ft_getenv("HOME"));
		else
			ft_error("cd", "HOME not set");
	}
	else
	{
		if (args[1][0] == '-' && args[1][1] == '\0')
		{
			if (ft_getenv("OLDPWD"))
			{
				status = chdir(ft_getenv("OLDPWD"));
				if (!status)
				{
					ft_putstr_fd(oldpwd, 1);
					ft_putstr_fd("\n", 1);
				}
			}
			else
				ft_error("cd", "OLDPWD not set");
		}
		else
			status = chdir(args[1]);
	}
	if (!status)
		ft_refreshpwds(oldpwd);
	else
		ft_error("cd", "No such file or directory");
	return (status);
}

int	ft_pwd(char **args)
{
	char	*pwd;

	getcwd(pwd, NULL);
	ft_putstr_fd(pwd, 1);
	ft_putstr_fd("\n", 1);
	return (0);
}

void	ft_addmatrixrow(char ***matrix, char *row)
{
	int		i;
	char	**newmatrix;

	if (!matrix || !(*matrix))
		return ;
	newmatrix = malloc((ft_matrixlen(*matrix) + 2) * sizeof(char *));
	i = 0;
	while (i < ft_matrixlen(*matrix))
	{
		newmatrix[i] = (*matrix)[i];
		i++;
	}
	newmatrix[i] = row;
	newmatrix[i + 1] = NULL;
	ft_freematrix(matrix);
	matrix = &newmatrix;
}

void	ft_removematrixrow(char ***matrix, char *row)
{
	int		i;
	int		j;
	char	**newmatrix;

	if (!matrix || !(*matrix))
		return ;
	newmatrix = malloc(ft_matrixlen(*matrix) * sizeof(char *));
	i = 0;
	while (i < ft_matrixlen(*matrix))
	{
		if (ft_strncmp(row, (*matrix)[i], ft_strlen(row)))
			newmatrix[i] = (*matrix)[j++];
		i++;
	}
	newmatrix[i] = NULL;
	ft_freematrix(matrix);
	matrix = &newmatrix;
}

void	ft_printenvironment(void)
{
	int	i;
	int	j;

	i = 0;
	while (g_data.env[i])
	{
		ft_putstr_fd("declare -x ", 1);
		j = 0;
		while (g_data.env[i][j])
		{
			ft_putchar_fd(g_data.env[i][j], 1);
			if (g_data.env[i][j++] == '=')
				break ;
		}
		if (g_data.env[i][j - 1] == '=')
		{
			ft_putstr_fd("\"", 1);
			while (g_data.env[i][j])
				ft_putchar_fd(g_data.env[i][j++], 1);
			ft_putstr_fd("\"\n", 1);
		}
		i++;
	}
}

int	ft_isdefined(char *variable)
{
	int		i;
	char	*name;
	int		ret;

	while (variable[i] && variable[i] != '=')
		i++;
	name = malloc(sizeof(char) * (i + 1));
	i = 0;
	while (variable[i] && variable[i] != '=')
	{
		name[i] = variable[i];
		i++;
	}
	name[i] = '\0';
	ret = ft_getenv(name);
	free(name);
	return (ret);	
}

int	ft_isvalididentifier(char *variable)
{
	int	i;

	i = 0;
	if (!(ft_isalpha(variable[i]) || variable[i] == '_'))
		return (0);
	while (variable[i] && variable[i] != '=')
	{
		if (!ft_isalnum(variable[i]))
			return (0);
		i++;
	}
	return (1);
}

int	ft_export(char **args)
{
	int	i;
	int	unsetargs[2];
	int	ret;

	if (ft_matrixlen(args) == 1)
		ft_printenvironment();
	else
	{
		i = 1;
		while (args[i])
		{
			if (ft_isvalididentifier(args[i]))
			{
				ft_putstr_fd("minishell: export: `", 1);
				ft_putstr_fd(args[i++], 1);
				ft_putstr_fd("': not a valid identifier", 1);
				ret = 1;
				continue ;
			}
			if (!ft_isdefined(args[i])) 	// not present, just add
				ft_addmatrixrow(&g_data.env, args[i]);
			else							// if present, overwrite
			{
				unsetargs[1] = args[i];
				ft_unset(unsetargs);
				ft_addmatrixrow(&g_data.env, args[i]);
				ret = 0;
			}
			i++;
		}
	}
	return (ret);
}

int	ft_unset(char **args)
{
	int	i;
	int	ret;

	if (ft_matrixlen(args) != 1)
	{
		i = 1;
		while (args[i])
		{
			if (ft_isvalididentifier(args[i]))
			{
				ft_putstr_fd("minishell: unset: `", 1);
				ft_putstr_fd(args[i++], 1);
				ft_putstr_fd("': not a valid identifier", 1);
				ret = 1;
				continue ;
			}
			if (ft_isdefined(args[i]))
			{
				ft_removematrixrow(&g_data.env, args[i]);
				ret = 0;
			}
			i++;
		}
	}
	return (ret);	
}