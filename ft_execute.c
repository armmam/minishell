#include "minishell.h"

void	ft_receive_heredoc(t_cmd *cmd)
{
	int		pipefd[2];
	int		refined;
	char	*stopword;
	char	*temp;

	refined = 1;
	stopword = cmd->heredoc;
	if (cmd->heredoc)
	{
		pipe(pipefd);
		if (cmd->in != 0)
			close(cmd->in);
		cmd->in = pipefd[0];
		if (ft_isquoted(cmd->heredoc, '\'') || ft_isquoted(cmd->heredoc, '\"'))
		{
			stopword[ft_strlen(stopword) - 1] = '\0';
			stopword++;
			refined = 0;
		}
		printf("HEREDOC IS |%s| (refined=%d)\n", cmd->heredoc, refined);
		while (1)
		{
			temp = readline("> ");
			if (!ft_strcmp(temp, stopword))
			{
				free(temp);
				printf("BROKE\n");
				close(pipefd[1]);
				break ;
			}
			if (refined)
				temp = ft_refineline(temp);
			ft_putstr_fd(temp, pipefd[1]);
			ft_putstr_fd("\n", pipefd[1]);
			free(temp);
		}
	}
}

void	ft_exec(t_cmd *cmd)
{
	int		i;
	char	*temp;
	char	*newpath;
	char	**paths;

	printf("INSIDE FT_EXEC: self%d; parent%d\n", getpid(), getppid());
	// receiving heredoc (<<)
	ft_receive_heredoc(cmd);
	// associating fds
	if (!ft_isbuiltin(cmd->args[0]) || g_data.cmds != 1)
	{
		int dupin = dup2(cmd->in, 0);
		int dupout = dup2(cmd->out, 1);
		dprintf(2, "cmd.in is %d and cmd.out is %d\n", cmd->in, cmd->out);
		if (dupin == -1 || dupout == -1)
			exit(EXIT_FAILURE);
	}
	// exec
	if (cmd->args[0][0] == '/')
	{
		execve(cmd->args[0], cmd->args, g_data.env->ptr);
		ft_error(cmd->args[0], "No such file or directory");
	}
	else
	{
		if (ft_execbuiltin(cmd) == -1)
		{
			if (ft_getenv("PATH"))
			{
				// try to find the command by iterating over paths in PATH
				paths = ft_split(ft_getenv("PATH"), ':');
				i = 0;
				while (paths[i])
				{
					temp = ft_strjoin(paths[i], "/");
					newpath = ft_strjoin(temp, cmd->args[0]);
					free(temp);
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
	if (!ft_isbuiltin(cmd->args[0]) || g_data.cmds != 1)
		exit(1);
	// printf("LEAVING FT_EXEC\n");
}