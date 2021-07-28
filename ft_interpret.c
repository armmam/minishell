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

t_cmd	*ft_parsecommands(char **tokens)
{
	int		i;
	t_cmd	*commands;
	int		pipefd[2];

	commands = malloc(sizeof(t_cmd) * g_data.family_size);
	i = 0;
	while (i < g_data.family_size)
	{
		ft_extractinfiles(&commands[i], tokens);
		ft_extractarguments(&commands[i], tokens);
		ft_extractoutfiles(&commands[i], tokens);
	}
	i = 0;
	while (i < g_data.family_size)
	{
		commands[i].in = 0;
		commands[i].out = 1;
		commands[i].err = 2;
		if (i)
		{
			pipe(pipefd);
			if (commands[i - 1].out != 1)
				close(pipefd[1]);
			else
				commands[i - 1].out = pipefd[1];
			if (commands[i].in != 0)
				close(pipefd[0]);
			else
				commands[i].in = pipefd[0];
		}
		i++;
	}
	return (commands);
}

void	ft_interpret(char *line)
{
	int		i;
	char	**tokens;
	t_cmd	*commands;

	// ! treatment of special characters is needed !
	tokens = ft_split(line, ' ');
	g_data.family_size = ft_cmdcount(tokens) + 1;
	g_data.family = malloc(sizeof(pid_t) * g_data.family_size);
	commands = ft_parsecommands(tokens);
	i = 0;
	while (i < g_data.family_size)
	{
		g_data.family[i] = fork();
		if (g_data.family[i] == 0) // child code
			ft_exec(&commands[i]);
		else if (g_data.family[i] < 0)
		{
			perror("minishell");
			while (--i >= 0)
				kill(g_data.family[i], SIGINT);
			break ;
		}
	}
	i = 0;
	while (i < g_data.family_size)
		waitpid(g_data.family[i++], &g_data.status, 0);
	
	// free stuff
	// ft_freecomands(commands); // ALSO CLOSE FDS
	if (g_data.family)
	{
		free(g_data.family);
		g_data.family = NULL;
	}
	if (tokens)
		ft_freematrix(tokens);
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

void	ft_abort(t_cmd *cmd)
{
	int	i;

	i = cmd->i + 1;
	while (i < g_data.family_size)
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
