#include "minishell.h"

void	ft_receive_heredoc(t_cmd *cmd, int k, int *write_ends)
{
	int		refined;
	char	*heredoc;
	char	*temp;
	size_t	j;

	if (cmd->heredoc)
	{
		j = 0;
		while (j < cmd->heredoc->len)
		{
			refined = 1;
			if (!ft_strcmp(cmd->refine->ptr[j], "\'") && j == cmd->heredoc->len - 1)
				refined = 0;
			// if (ft_isquoted(cmd->heredoc->ptr[j], '\'') && j == cmd->heredoc->len - 1)
			// if (ft_isquoted(cmd->heredoc->ptr[j], '\'') || ft_isquoted(cmd->heredoc->ptr[j], '\"'))
			// {
			// 	heredoc = cmd->heredoc->ptr[j] + 1;
			// 	heredoc[ft_strlen(heredoc) - 1] = '\0';
			// }
			heredoc = cmd->heredoc->ptr[j];
			j++;
			while (1)
			{
				temp = readline("> ");
				if (temp == NULL)
					break ;
				else if (!ft_strcmp(temp, heredoc))
				{
					free(temp);
					if (j == cmd->heredoc->len)
						close(write_ends[k]);
					break ;
				}
				if (refined)
					temp = ft_refineline(temp);
				if (j == cmd->heredoc->len) // we're waiting for the last heredoc
				{
					ft_putstr_fd(temp, write_ends[k]);
					ft_putstr_fd("\n", write_ends[k]);
				}
				free(temp);
			}
		}
	}
}

/*
 * returns the number of heredocs, saves the write-ends
 * of the heredoc
 */
int	ft_save_write_ends(int **write_ends)
{
	int	i;
	int	j;
	int	count;
	int	pipefd[2];

	count = 0;
	i = 0;
	while (i < g_data.cmds)
		if (g_data.commands[i++].heredoc)
			count++;
	*write_ends = malloc(count * sizeof(int *));
	i = 0;
	j = 0;
	while (i < g_data.cmds)
	{
		if (g_data.commands[i].heredoc)
		{
			pipe(pipefd);
			if (g_data.commands[i].in != 0)
				close(g_data.commands[i].in);
			g_data.commands[i].in = pipefd[0];
			(*write_ends)[j++] = pipefd[1];
		}
		i++;
	}
	return (count);
}

/*
 * launches a process for receiveng heredoc;
 * if it was terminated by a SIGINT signal, returns 1;
 * else if exits successfully, returns 0 
 */
int	ft_launch_heredoc(void)
{
	int	i;
	int	heredocs;
	int	*write_ends;
	int	heredoc_pid;
	int	heredoc_status;

	write_ends = NULL;
	heredocs = ft_save_write_ends(&write_ends);
	if (!write_ends)
		return (0);
	ft_ignore_signals();
	heredoc_pid = fork();
	i = 0;
	while (heredoc_pid && i < heredocs)
		close(write_ends[i++]);
	heredocs = 0;
	if (heredoc_pid == 0) // heredoc code section
	{
		printf("HEREDOC PID: %d\n", getpid());
		ft_heredoc_signals();
		i = 0;
		while (i < g_data.cmds)
		{
			if (g_data.commands[i].heredoc)
				ft_receive_heredoc(&g_data.commands[i], heredocs++, write_ends);
			i++;
		}
		exit(0);
	}
	wait(&heredoc_status);
	free(write_ends);
	ft_define_signals();
	if (WEXITSTATUS(heredoc_status) == 1)
	{
		g_data.status = 1;
		ft_putstr_fd("\n", 1);
		return (1);
	}
	return (0);
}