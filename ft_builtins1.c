#include "minishell.h"

int	ft_abs(int nb)
{
	if (nb < 0)
		return (-nb);
	return (nb);
}

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

void	ft_refresh_pwds(char *oldpwd)
{
	t_cmd	export;
	char	*pwd;

	pwd = getcwd(NULL, 0);
	export.args = ft_calloc(3, sizeof(char *));
	export.args[1] = ft_strjoin("PWD=", pwd);
	ft_export(&export);
	ft_freematrix(export.args);
	free(pwd);
	export.args = ft_calloc(3, sizeof(char *));
	export.args[1] = ft_strjoin("OLDPWD=", oldpwd);
	ft_export(&export);
	ft_freematrix(export.args);
}

int	ft_cd(t_cmd *cmd)
{
	int		status;
	char	*oldpwd;

	status = 1;
	oldpwd = getcwd(NULL, 0);
	if (ft_matrixlen(cmd->args) == 1)
	{
		if (ft_getenv("HOME"))
			status = ft_abs(chdir(ft_getenv("HOME")));
		else
			status = ft_error("cd", "HOME not set");
	}
	else
	{
		if (cmd->args[1][0] == '-' && cmd->args[1][1] == '\0')
		{
			if (ft_getenv("OLDPWD"))
			{
				status = ft_abs(chdir(ft_getenv("OLDPWD")));
				if (!status)
				{
					ft_putstr_fd(oldpwd, cmd->out);
					ft_putstr_fd("\n", cmd->out);
				}
			}
			else
				status = ft_error("cd", "OLDPWD not set");
		}
		else
			status = ft_abs(chdir(cmd->args[1]));
	}
	if (!status)
		ft_refresh_pwds(oldpwd);
	else
		status = ft_error("cd", "No such file or directory\n");
	free(oldpwd);
	return (status);
}

int	ft_pwd(t_cmd *cmd)
{
	char	*pwd;

	pwd = getcwd(NULL, 0);
	ft_putstr_fd(pwd, cmd->out);
	ft_putstr_fd("\n", cmd->out);
	free(pwd);
	return (0);
}

void	ft_print_environment(int fd)
{
	int	i;
	int	j;

	i = 0;
	while (g_data.env->ptr[i])
	{
		ft_putstr_fd("declare -x ", fd);
		j = 0;
		while (g_data.env->ptr[i][j])
		{
			ft_putchar_fd(g_data.env->ptr[i][j], fd);
			if (g_data.env->ptr[i][j++] == '=')
				break ;
		}
		if (g_data.env->ptr[i][j - 1] == '=')
		{
			ft_putstr_fd("\"", fd);
			while (g_data.env->ptr[i][j])
				ft_putchar_fd(g_data.env->ptr[i][j++], fd);
			ft_putstr_fd("\"\n", fd);
		}
		i++;
	}
}

/*
 * takes in a declaration; returns 1 if it is already defined,
 * 0 otherwise 
 */
char	*ft_isdefined(char *decl)
{
	char	*ret;
	char	*name;

	name = ft_separate_identifier(decl);
	ret = ft_getenv(name);
	free(name);
	return (ret);	
}

/*
 * goes through a string and checks whether
 * it's a valid identifier. returns 1 if it is,
 * 0 otherwise
 */
int	ft_isvalididentifier(const char *name)
{
	int	i;

	i = 0;
	if (!name)
		return (0);
	if (!(ft_isalpha(name[i]) || name[i] == '_') || name[i] == '=')
		return (0);
	i++;
	while (name[i])
	{
		if (!(ft_isalnum(name[i]) || name[i] == '_') || name[i] == '=')
			return (0);
		i++;
	}
	return (1);
}

/*
 * checks whether a declaration's varname is valid
 */
int	ft_isvaliddeclaration(char *decl)
{
	int		ret;
	char	*name;

	name = ft_separate_identifier(decl);
	ret = ft_isvalididentifier(name);
	free(name);
	return (ret);
}

/*
 * takes a declaration string, separates
 * the identifier before the first '='
 * and returns it. must be freed later
 */
char	*ft_separate_identifier(char *decl)
{
	int		i;
	char	*name;
	
	i = 0;
	while (decl[i] && decl[i] != '=')
		i++;
	name = ft_calloc(i + 1, sizeof(char));
	i = 0;
	while (decl[i] && decl[i] != '=')
	{
		name[i] = decl[i];
		i++;
	}
	name[i] = '\0';
	return (name);
}

int	ft_export(t_cmd *cmd)
{
	int		i;
	int		ret;
	t_cmd	unset;

	ret = 0;
	if (ft_matrixlen(cmd->args) == 1)
		ft_print_environment(cmd->out);
	else
	{
		i = 1;
		while (cmd->args[i])
		{
			if (!ft_isvaliddeclaration(cmd->args[i]))
			{
				ft_putstr_fd("minishell: export: `", 2);
				ft_putstr_fd(cmd->args[i++], 2);
				ft_putstr_fd("': not a valid identifier\n", 2);
				ret = 1;
				continue ;
			}
			if (!ft_isdefined(cmd->args[i])) 	// not present, just add
				ft_darrpushback(g_data.env, cmd->args[i]);
			else							// if present, overwrite
			{
				unset.args = ft_calloc(3, sizeof(char *));
				unset.args[1] = ft_separate_identifier(cmd->args[i]);
				ret = ft_unset(&unset);
				ft_darrpushback(g_data.env, cmd->args[i]);
				ft_freematrix(unset.args);
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
			if (!ft_isvalididentifier(cmd->args[i]))
			{
				ft_putstr_fd("minishell: unset: `", 2);
				ft_putstr_fd(cmd->args[i++], 2);
				ft_putstr_fd("': not a valid identifier\n", 2);
				ret = 1;
				continue ;
			}
			if (ft_isdefined(cmd->args[i]))
			{
				ft_darrerase(g_data.env, ft_getenv_full(cmd->args[i]));
				ret = 0;
			}
			i++;
		}
	}
	return (ret);
}