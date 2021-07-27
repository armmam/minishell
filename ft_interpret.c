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
		// IF an absolute path is specified
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
	ft_freecharmatrix(args);
}

// RAW
int	ft_execbuiltin(char **args)
{
	(void)args;
	// some code...
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
	ft_freecharmatrix(paths);
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