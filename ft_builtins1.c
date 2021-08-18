#include "minishell.h"

// can this command ever exit with status=1?
int	ft_echo(t_cmd *cmd)
{
	char	**arg;
	int		nl; // newline

	arg = (cmd->args) + 1;
	nl = 1;
	if (!ft_strcmp(*arg, "-n"))
	{
		nl = 0;
		arg++;
	}
	if (!*arg)
	{
		ft_putstr_fd("\n", cmd->out);
		return (0);
	}
	while (*arg)
	{
		ft_putstr_fd((*arg)++, cmd->out);
		if (*arg)
			ft_putstr_fd(" ", cmd->out);
	}
	if (nl)
		ft_putstr_fd("\n", cmd->out);
	return (0);
}

void	ft_refreshpwds(char *oldpwd)
{
	t_cmd	export;
	char	**exportargs;
	char	*pwd;

	exportargs = ft_calloc(3, sizeof(char *));
	export.args = exportargs;
	pwd = NULL;
	getcwd(pwd, 0);
	export.args[1] = ft_strjoin("PWD=", pwd);
	export.args[2] = NULL;
	ft_export(&export);
	export.args[1] = ft_strjoin("OLDPWD=", oldpwd);
	ft_export(&export);
}

int	ft_cd(t_cmd *cmd)
{
	int		status;
	char	*oldpwd;

	status = 1;
	oldpwd = NULL;
	getcwd(oldpwd, 0);
	if (ft_matrixlen(cmd->args) == 1)
	{
		if (ft_getenv("HOME"))
			status = chdir(ft_getenv("HOME"));
		else
			ft_error("cd", "HOME not set");
	}
	else
	{
		if (cmd->args[1][0] == '-' && cmd->args[1][1] == '\0')
		{
			if (ft_getenv("OLDPWD"))
			{
				status = chdir(ft_getenv("OLDPWD"));
				if (!status)
				{
					ft_putstr_fd(oldpwd, cmd->out);
					ft_putstr_fd("\n", cmd->out);
				}
			}
			else
				ft_error("cd", "OLDPWD not set");
		}
		else
			status = chdir(cmd->args[1]);
	}
	if (!status)
		ft_refreshpwds(oldpwd);
	else
		ft_error("cd", "No such file or directory");
	return (status);
}

int	ft_pwd(t_cmd *cmd)
{
	char	*pwd;

	pwd = NULL;
	getcwd(pwd, 0);
	ft_putstr_fd(pwd, cmd->out);
	ft_putstr_fd("\n", cmd->out);
	return (0);
}

void	ft_addmatrixrow(char ***matrix, char *row)
{
	int		i;
	char	**newmatrix;

	if (!matrix || !(*matrix))
		return ;
	newmatrix = ft_calloc(ft_matrixlen(*matrix) + 2, sizeof(char *));
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
	newmatrix = ft_calloc(ft_matrixlen(*matrix), sizeof(char *));
	i = 0;
	j = 0;
	while (i < ft_matrixlen(*matrix))
	{
		if (ft_strncmp(row, (*matrix)[i], ft_strlen(row))) // typo: their lengths have to be equal also
			newmatrix[i] = (*matrix)[j++];
		i++;
	}
	newmatrix[i] = NULL;
	ft_freematrix(matrix);
	matrix = &newmatrix;
}

void	ft_printenvironment(int fd)
{
	int	i;
	int	j;

	i = 0;
	while (g_data.env[i])
	{
		ft_putstr_fd("declare -x ", fd);
		j = 0;
		while (g_data.env[i][j])
		{
			ft_putchar_fd(g_data.env[i][j], fd);
			if (g_data.env[i][j++] == '=')
				break ;
		}
		if (g_data.env[i][j - 1] == '=')
		{
			ft_putstr_fd("\"", fd);
			while (g_data.env[i][j])
				ft_putchar_fd(g_data.env[i][j++], fd);
			ft_putstr_fd("\"\n", fd);
		}
		i++;
	}
}

char	*ft_isdefined(char *variable)
{
	int		i;
	char	*name;
	char	*ret;

	i = 0;
	while (variable[i] && variable[i] != '=')
		i++;
	name = ft_calloc(i + 1, sizeof(char));
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
		if (!(ft_isalnum(variable[i]) || variable[i] == '_'))
			return (0);
		i++;
	}
	return (1);
}

int	ft_export(t_cmd *cmd)
{
	int		i;
	t_cmd	unset;
	char	**unsetargs;
	int		ret;

	ret = 0;
	if (ft_matrixlen(cmd->args) == 1)
		ft_printenvironment(cmd->out);
	else
	{
		i = 1;
		while (cmd->args[i])
		{
			if (ft_isvalididentifier(cmd->args[i]))
			{
				ft_putstr_fd("minishell: export: `", 2);
				ft_putstr_fd(cmd->args[i++], 2);
				ft_putstr_fd("': not a valid identifier", 2);
				ret = 1;
				continue ;
			}
			if (!ft_isdefined(cmd->args[i])) 	// not present, just add
				ft_addmatrixrow(&g_data.env, cmd->args[i]);
			else							// if present, overwrite
			{
				unsetargs = ft_calloc(3, sizeof(char *));
				unsetargs[1] = cmd->args[i];
				unsetargs[2] = NULL;
				unset.args = unsetargs;
				ft_unset(&unset);
				ft_addmatrixrow(&g_data.env, cmd->args[i]);
				ret = 0;
			}
			i++;
		}
	}
	return (ret);
}

int	ft_unset(t_cmd *cmd)
{
	int	i;
	int	ret;

	ret = 0;
	if (ft_matrixlen(cmd->args) != 1)
	{
		i = 1;
		while (cmd->args[i])
		{
			if (ft_isvalididentifier(cmd->args[i]))
			{
				ft_putstr_fd("minishell: unset: `", 2);
				ft_putstr_fd(cmd->args[i++], 2);
				ft_putstr_fd("': not a valid identifier", 2);
				ret = 1;
				continue ;
			}
			if (ft_isdefined(cmd->args[i]))
			{
				ft_removematrixrow(&g_data.env, cmd->args[i]);
				ret = 0;
			}
			i++;
		}
	}
	return (ret);	
}