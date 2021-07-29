#include "minishell.h"

int	ft_cmdcount(char **tokens)
{
	int	i;
	int	count;

	count = 0;
	i = 0;
	while (tokens[i])
	{
		if (tokens[i][0] == '|' && tokens[i][1] == '\0')
			count++;
		if (tokens[i][0] == '&' && tokens[i][1] == '&'
			&& tokens[i][2] == '\0')
			count++;
		if (tokens[i][0] == '|' && tokens[i][1] == '|'
			&& tokens[i][2] == '\0')
			count++;
		i++;
	}
	return (count);
}

void	ft_extractinfiles(t_cmd *cmd, char **tokens)
{
	(void)cmd;
	(void)tokens;
	//
}

void	ft_extractoutfiles(t_cmd *cmd, char **tokens)
{
	(void)cmd;
	(void)tokens;
	//
}

t_cmd	*ft_parsecmds(char **tokens)
{
	int		i;
	t_cmd	*cmds;
	int		pipefd[2];

	cmds = malloc(sizeof(t_cmd) * g_data.cmdcount);
	i = 0;
	while (i < g_data.cmdcount)
	{
		cmds[i].i = i;
		ft_extractinfiles(&cmds[i], tokens);
		ft_extractarguments(&cmds[i], tokens);
		ft_extractoutfiles(&cmds[i], tokens);
		i++;
	}
	i = 0;
	while (i < g_data.cmdcount)
	{
		cmds[i].in = 0;
		cmds[i].out = 1;
		cmds[i].err = 2;
		if (i)
		{
			pipe(pipefd);
			if (cmds[i - 1].out != 1)
				close(pipefd[1]);
			else
				cmds[i - 1].out = pipefd[1];
			if (cmds[i].in != 0)
				close(pipefd[0]);
			else
				cmds[i].in = pipefd[0];
		}
		i++;
	}
	return (cmds);
}

void	ft_interpret(char *line)
{
	int		i;
	int		j;
	int		processes;
	char	**tokens;
	t_cmd	*cmds;

	// ! treatment of special characters is needed !
	tokens = ft_tokenize(line);
	//
	g_data.cmdcount = ft_cmdcount(tokens);
	cmds = ft_parsecmds(tokens);
	processes = g_data.cmdcount - ft_isbuiltin(cmds[0].args[0])
		- ft_isbuiltin(cmds[g_data.cmdcount - 1].args[0]);
	g_data.family = malloc(sizeof(pid_t) * processes);
	i = 0;
	j = 0;
	while (i < g_data.cmdcount)
	{
		if ((!i || i == g_data.cmdcount - 1) && ft_isbuiltin(cmds[i].args[0]))
			ft_exec(&cmds[i]);
		else
		{
			g_data.family[j] = fork();
			if (g_data.family[j] == 0) // child code
				ft_exec(&cmds[i]);
			else if (g_data.family[j] < 0)
			{
				perror("minishell");
				while (--j >= 0)
					kill(g_data.family[j], SIGINT);
				break ;
			}
			j++;
		}
		i++;
	}
	i = 0;
	while (i < g_data.cmdcount)
		waitpid(g_data.family[i++], &g_data.status, 0);
	
	// free stuff
	// ft_freecomands(cmds); // ALSO CLOSE FDS
	if (g_data.family)
	{
		free(g_data.family);
		g_data.family = NULL;
	}
	if (tokens)
		ft_freematrix(tokens);
}

int	ft_isbuiltin(char *builtin)
{
	if (ft_convertbuiltin(builtin))
		return (1);
	return (0);
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
	return (0);
}

int	ft_execbuiltin(t_cmd *cmd)
{
	int	cmd;

	// do the dup2s!!

	cmd = ft_convertbuiltin(cmd->args[0]);
	if (cmd == __echo)
		g_data.status = ft_echo(cmd->args);
	else if (cmd == __cd)
		g_data.status = ft_cd(cmd->args);
	else if (cmd == __pwd)
		g_data.status = ft_pwd(cmd->args);
	else if (cmd == __export)
		g_data.status = ft_export(cmd->args);
	else if (cmd == __unset)
		g_data.status = ft_unset(cmd->args);
	else if (cmd == __env)
		g_data.status = ft_env(cmd->args);
	else if (cmd == __exit)
		exit(0);
	return (0);
}

void	ft_abort(t_cmd *cmd)
{
	int	i;

	i = cmd->i + 1;
	while (i < g_data.cmdcount)
		kill(g_data.family[i++], SIGINT);
	if (cmd->cond == or)
		exit(0);
	else
		exit(1);
}

void	ft_exec(t_cmd *cmd)
{
	int		i;
	char	*temp;
	char	*newpath;
	char	**paths;
	int		status;

	// associating fds
	if (dup2(cmd->in, 0) == -1 || dup2(cmd->out, 1) == -1
		|| dup2(cmd->err, 2) == -1)
		ft_abort(cmd);
	// wait for the previous one
	if (cmd->i)
	{
		waitpid(g_data.family[cmd->i - 1], &status, 0);
		if ((status && cmd->cond == and) || (!status && cmd->cond == or))
			ft_abort(cmd);
	}
	// exec
	if (cmd->args[0][0] == '/')
	{
		if (execve(cmd->args[0], cmd->args, g_data.env) == -1)
			ft_error(cmd->args[0], "No such file or directory");
	}
	else
	{
		if (!ft_execbuiltin(cmd->args))
		{
			if (ft_getenv("PATH"))
			{
				paths = ft_split(ft_getenv("PATH"), ':');
				i = 0;
				while (paths[i])
				{
					temp = ft_strjoin(paths[i], "/");
					newpath = ft_strjoin(temp, cmd->args[0]);
					execve(newpath, cmd->args, g_data.env);
					free(newpath);
					free(temp);
					i++;
				}
				ft_freematrix(paths);
				ft_error(cmd->args[0], "no such command found");
			}
			else
				ft_error(cmd->args[0], "No such file or directory");
		}
	}
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
