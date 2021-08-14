#include "minishell.h"

int	ft_cmds(char **tokens)
{
	int	i;
	int	count;

	if (!tokens)
		return (0);
	count = 1;
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

// puts char **args into given command's args field;
// opens the files given as <, > or >> with open()
// and writes them into the in and out fields (don't
// forget that if multiple outfiles are received,
// they have to be opened and closed except for the last
// one. store only the last one)
// parses 'heredoc' if applicable; else assigns NULL to it
void	ft_extractarguments(t_cmd *cmd, char **tokens)
{
	(void)cmd;
	(void)tokens;
}

t_cmd	*ft_parsecommands(char **tokens)
{
	int		i;
	t_cmd	*commands;
	int		pipefd[2];

	commands = malloc(sizeof(t_cmd) * g_data.cmds);
	i = 0;
	while (i < g_data.cmds)
	{
		commands[i].in = 0;
		commands[i].out = 1;
		ft_extractarguments(&commands[i], tokens);
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
	int		j;
	t_cmd	*commands;


	// TAKE CARE OF THIS COMMENT BLOCK AND UNCOMMENT IT
	// ! treatment of special characters is needed !
	// tokens = ft_tokenize(line);
	// g_data.cmds = ft_cmds(tokens);
	// commands = ft_parsecommands(tokens);


	g_data.prcs = g_data.cmds - ft_isbuiltin(commands[0].args[0]);
	g_data.family = malloc(sizeof(pid_t) * g_data.prcs);
	i = 0;
	j = 0;
	while (i < g_data.cmds)
	{
		if (g_data.cmds == 1 && ft_isbuiltin(commands[0].args[0]))
			ft_exec(&commands[i]);
		else
		{
			g_data.family[j] = fork();
			if (g_data.family[j] == 0) // child code
				ft_exec(&commands[i]);
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
	while (i < g_data.prcs)
		waitpid(g_data.family[i++], &g_data.status, 0);
	
	// free stuff
	// ft_freecomands(commands); // ALSO CLOSE FDS
	if (g_data.family)
	{
		free(g_data.family);
		g_data.family = NULL;
	}
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
	int	ret;

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
		exit(0);
	if (ret)
		return (0);
	return (-1);
}

int		ft_isquoted(char *str, char c)
{
	if (str[0] == c && str[ft_strlen(str) - 1] == c)
		return (1);
	return (0);
}

void	ft_abort(t_cmd *cmd)
{
	int	i;

	i = cmd->i + 1;
	while (i < g_data.prcs)
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

	// wait for the previous one
	if (cmd->i)
	{
		waitpid(g_data.family[cmd->i - 1], &status, 0);
		if ((status && cmd->cond == and) || (!status && cmd->cond == or))
			ft_abort(cmd);
	}
	// associating fds
	if (!ft_isbuiltin(cmd->args[0]) || g_data.cmds != 1)
		if (dup2(cmd->in, 0) == -1 || dup2(cmd->out, 1) == -1)
			ft_abort(cmd);
	// receiving heredoc (<<)
	if (cmd->heredoc)
	{
		int	refined = 0;
		if (!(ft_isquoted(cmd->heredoc, '\'') || ft_isquoted(cmd->heredoc, '\"')))
		{
			cmd->heredoc[ft_strlen(cmd->heredoc) - 1] = '\0';
			cmd->heredoc++;
			refined = 1;
		}
		while (1)
		{
			temp = readline("> ");
			if (ft_strlen(cmd->heredoc) == ft_strlen(temp)
				&& !ft_strncmp(temp, cmd->heredoc, ft_strlen(cmd->heredoc)))
			{
				free(temp);
				break ;
			}
			if (refined)
				temp = ft_refineline(temp);
			ft_putstr_fd(temp, cmd->in);
			ft_putstr_fd("\n", cmd->in);
			free(temp);
		}
	}
	// exec
	if (cmd->args[0][0] == '/')
	{
		execve(cmd->args[0], cmd->args, g_data.env) == -1;
		ft_error(cmd->args[0], "No such file or directory");
		exit(1);
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
					execve(newpath, cmd->args, g_data.env);
					free(newpath);
					i++;
				}
				ft_freematrix(&paths);
				ft_error(cmd->args[0], "No such command found");
				exit(1);
			}
			else
			{
				ft_error(cmd->args[0], "No such file or directory");
				exit(1);
			}
		}
	}
}

void	ft_error(char *name, char *desc)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(name, 2);
	ft_putstr_fd(": ", 2);
	ft_putstr_fd(desc, 2);
	ft_putstr_fd("\n", 2);
}
