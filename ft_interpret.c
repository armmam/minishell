#include "minishell.h"

void	ft_interpret(char *line)
{
	char	**args;
	char	**paths;
	char	*path;
	pid_t	pid;

	// ! treatment of special characters is needed !
	args = ft_split(line, ' ');
	pid = fork();
	if (pid == 0)
	{
		if (args[0][0] == '/')
		{
			if (execve(args[0], args, g_data.env) == -1)
				ft_error(args[0], "No such file or directory");
		}
		else
		{
			if (!ft_execbuiltin(args))
			{
				if (ft_getenv("PATH"))
				{
					path = ft_getenv("PATH");
					path += 5;
					paths = ft_split(path, ':');
					ft_exec(args, paths);
				}
				else
					ft_error(args[0], "No such file or directory");
			}
		}
	}
	else if (pid < 0)
		perror("minishell");
	else
		waitpid(pid, NULL, 0);
	ft_freematrix(args);
}

int	ft_convertbuiltin(char *builtin)
{
	if (ft_strlen(builtin) == 4 && !ft_strncmp(builtin, "echo", 4))
		return (__echo);
	else if (ft_strlen(builtin) == 2 && !ft_strncmp(builtin, "cd", 2))
		return (__cd);
	else if (ft_strlen(builtin) == 3 && !ft_strncmp(builtin, "pwd", 3))
		return (__pwd);
	else if (ft_strlen(builtin) == 6 && !ft_strncmp(builtin, "export", 6))
		return (__export);
	else if (ft_strlen(builtin) == 5 && !ft_strncmp(builtin, "unset", 5))
		return (__unset);
	else if (ft_strlen(builtin) == 3 && !ft_strncmp(builtin, "env", 3))
		return (__env);
	else if (ft_strlen(builtin) == 4 && !ft_strncmp(builtin, "exit", 4))
		return (__exit);
	return (-1);
}

int	ft_execbuiltin(char **args)
{
	int	cmd;

	cmd = ft_convertbuiltin(args[0]);
	if (cmd == __echo)
		return (ft_echo(args));
	else if (cmd == __cd)
		return (ft_cd(args));
	else if (cmd == __pwd)
		return (ft_pwd(args));
	else if (cmd == __export)
		return (ft_export(args));
	else if (cmd == __unset)
		return (ft_unset(args));
	else if (cmd == __env)
		return (ft_env(args));
	else if (cmd == __exit)
		return (ft_exit(args));
	return (0);
}

void	ft_exec(char **args, char **paths)
{
	int		i;
	char	*temp;
	char	*newpath;

	i = 0;
	while (paths[i])
	{
		temp = ft_strjoin(paths[i], "/");
		newpath = ft_strjoin(temp, args[0]);
		execve(newpath, args, g_data.env);
		free(newpath);
		free(temp);
		i++;
	}
	ft_freematrix(paths);
	ft_error(args[0], "no such command found");
}

int	ft_error(char *name, char *desc)
{
	ft_putstr_fd("minishell: ", 1);
	ft_putstr_fd(name, 1);
	ft_putstr_fd(": ", 1);
	ft_putstr_fd(desc, 1);
	ft_putstr_fd("\n", 1);
	exit(1);
}