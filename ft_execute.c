#include "minishell.h"

void	ft_receive_heredoc(t_cmd *cmd)
{
	int	pipefd[2];
	int	refined;
	char	*heredoc;
	char	*temp;

	if (cmd->heredoc)
	{
		pipe(pipefd);
		if (cmd->in != 0)
			close(cmd->in);
		cmd->in = pipefd[0];
		size_t j = 0;
		while (j < cmd->heredoc->len)
		{
			refined = 1;
			if (ft_isquoted(cmd->heredoc->ptr[j], '\''))
				refined = 0;
			if (ft_isquoted(cmd->heredoc->ptr[j], '\'') || ft_isquoted(cmd->heredoc->ptr[j], '\"'))
			{
				heredoc = cmd->heredoc->ptr[j] + 1;
				heredoc[ft_strlen(heredoc) - 1] = '\0';
			}
			else
				heredoc = cmd->heredoc->ptr[j];
			j++;
			while (1)
			{
				temp = readline("> ");
				if (!ft_strcmp(temp, heredoc))
				{
					free(temp);
					if (j == cmd->heredoc->len)
						close(pipefd[1]);
					break ;
				}
				if (refined)
					temp = ft_refineline(temp);
				if (j == cmd->heredoc->len) // we're waiting for the last heredoc
				{
					ft_putstr_fd(temp, pipefd[1]);
					ft_putstr_fd("\n", pipefd[1]);
				}
				free(temp);
			}
		}
	}
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
			ft_error(cmd->args[0], "No such command found");
		}
		else
			ft_error(cmd->args[0], "No such file or directory");
	}
}

void	ft_exec(t_cmd *cmd)
{
	printf("INSIDE FT_EXEC: self%d; parent%d\n", getpid(), getppid());
	// associating fds
	if (!ft_isbuiltin(cmd->args[0]) || g_data.cmds != 1)
	{
		if (dup2(cmd->in, 0) == -1 || dup2(cmd->out, 1) == -1)
		{
			perror("minishell");
			exit(1);
		}
		ft_close_descriptors();
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