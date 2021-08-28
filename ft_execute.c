#include "minishell.h"

int	ft_execbuiltin(t_cmd *cmd)
{
	int	ret;

	//if (parentid == getpid())
	//{
	//	dprintf(2, "command info in MAIN process: in%d, out%d\n", cmd->in, cmd->out);
	//	dprintf(2, "printing cmd's args:\n");
	//	int iter = 0;
	//	while (cmd->args[iter])
	//	{
	//		printf("%i:%s\n", iter, cmd->args[iter]);
	//		iter++;
	//	}
	//	printf("\n");
	//}
	ret = ft_convertbuiltin(cmd->args[0]);
	if (ret == __echo)
		g_data.status = ft_echo(cmd);
	else if (ret == __cd)
		g_data.status = ft_cd(cmd);
	else if (ret == __pwd)
		g_data.status = ft_pwd(cmd);
	else if (ret == __export)
		g_data.status = ft_export(cmd);
	else if (ret == __unset)
		g_data.status = ft_unset(cmd);
	else if (ret == __env)
		g_data.status = ft_env(cmd);
	else if (ret == __exit)
	{
		ft_putstr_fd("exit\n", 2);
		exit(0);
	}
	if (ret)
		return (0);
	return (-1);
}

void	ft_close_descriptors(void)
{
	int	i;

	i = 0;
	while (i < g_data.cmds)
	{
		if (g_data.commands[i].in != 0)
			close(g_data.commands[i].in);
		if (g_data.commands[i].out != 1)
			close(g_data.commands[i].out);
		i++;
	}
}

void	ft_traverse_binaries(t_cmd *cmd)
{
	int		i;
	char	*newpath;
	char	**paths;

	if (ft_execbuiltin(cmd) == -1)
	{
		execve(cmd->args[0], cmd->args, g_data.env->ptr);	// for binaries not in bin
		if (ft_getenv("PATH"))
		{
			paths = ft_split(ft_getenv("PATH"), ':');
			i = 0;
			while (paths[i])
			{
				newpath = ft_strjoin3(paths[i], "/", cmd->args[0]);
				execve(newpath, cmd->args, g_data.env->ptr);
				free(newpath);
				i++;
			}
			ft_freematrix(paths);
			ft_error(cmd->args[0], "command not found");
		}
		else
			ft_error(cmd->args[0], "No such file or directory");
	}
}

void	ft_exec(t_cmd *cmd)
{
	// printf("INSIDE FT_EXEC: self%d; parent%d\n", getpid(), getppid());
	// dprintf(2, "in:%d out:%d\n", cmd->in, cmd->out);
	// associating fds
	if (!ft_isbuiltin(cmd->args[0]) || g_data.cmds != 1)
	{
		if (dup2(cmd->in, 0) == -1 || dup2(cmd->out, 1) == -1)
		{
			perror("minishell");
			exit(1);
		}
		ft_close_descriptors();
		if (ft_isbuiltin(cmd->args[0]))
			cmd->out = 1;
	}
	// exec
	if (cmd->args[0][0] == '/')
	{
		execve(cmd->args[0], cmd->args, g_data.env->ptr);
		ft_error(cmd->args[0], "No such file or directory");
	}
	else
		ft_traverse_binaries(cmd);
	if (!(g_data.cmds == 1 && ft_isbuiltin(cmd->args[0])))
	{
		if (ft_isbuiltin(cmd->args[0]))
			exit(g_data.status);
		exit(127);
	}
}