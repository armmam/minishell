#include "minishell.h"

int	ft_cmds(char **tokens)
{
	int	i;
	int	count;

	if (!tokens || !(*tokens))
		return (0);
	count = 1;
	i = 0;
	printf("ABOUT TO START COUNTING\n");
	while (tokens[i])
	{
		if (tokens[i][0] == '|' && tokens[i][1] == '\0')
			count++;
		i++;
	}
	return (count);
}

/*
 * frees commands
 */
void	ft_freecomands(t_cmd *cmds)
{
	int	i;

	i = 0;
	while (i < g_data.cmds)
	{
		if (cmds[i].heredoc)
			free(cmds[i].heredoc);
		printf("FREED HEREDOC\n");
		if (cmds[i].args)
			ft_freematrix(cmds[i].args);
		printf("FREED ARGS\n");
		i++;
	}
	free(cmds);
}

/*
 * returns 0 upon success, 1 upon failure
 */
int		ft_parseheredoc(char ***tokens, char **heredoc)
{
	(*tokens)++;
	if (*tokens == NULL || ((*tokens)[0][0] == '|' && (*tokens)[0][1] == '\0')) // << is the last token in the command
	{
		ft_error(*(*tokens - sizeof(char *)), "syntax error");
		return (1);
	}
	if (*heredoc) // if already encountered any heredocs
		free(*heredoc); // then free memory allocated for it
	*heredoc = ft_strdup(**tokens);
	return (0);
}

/*
 * returns 0 upon success, 1 upon failure
 */
int		ft_parsefiletoken(char ***tokens, int *cmd_fd, int open_flag)
{
	int		fd;

	(*tokens)++;
	if (*tokens == NULL || ((*tokens)[0][0] == '|' && (*tokens)[0][1] == '\0')) // </>>/> is the last token in the command
	{
		ft_error(*(*tokens - 1), "syntax error");
		return (1);
	}
	if (*cmd_fd != 0) // if already encoutered </>>/> before for this command
		close(*cmd_fd);
	if ((fd = open(**tokens, open_flag)) == -1)
	{
		perror("minishell");
		return (1);
	}
	*cmd_fd = fd;
	return (0);
}

// puts char **args into given command's args field;
// opens the files given as <, > or >> with open()
// and writes them into the in and out fields (don't
// forget that if multiple outfiles are received,
// they have to be opened and closed except for the last
// one. store only the last one)
// parses 'heredoc' if applicable; else assigns NULL to it -- it's null because of ft_calloc
//
// returns a pointer to the next command in `tokens`
char	**ft_extractarguments(t_cmd *cmd, char **tokens)
{
	int		err;
	t_darr	*args;

	printf("TOKENS RECEIVED %p\n", tokens);
	printf("TOKENS RECEIVED (dereferenced) %p\n", *tokens);
	args = ft_darrnew(0);
	err = 0;
	while (tokens && *tokens && !((*tokens)[0] == '|' && (*tokens)[1] == '\0')) // haven't run out of tokens and haven't encountered a pipe
	{
		printf("TOKENS IN ft_extractarguments %p\n", tokens);
		printf("TOKEN:|%s| at %p, its address is %p\n", *tokens, *tokens, tokens);
		if (!ft_strcmp(*tokens, "<<"))  // if encountered <<
			err = ft_parseheredoc(&tokens, &(cmd->heredoc));
		else if (!ft_strcmp(*tokens, ">"))  // if encoutered >
			err = ft_parsefiletoken(&tokens, &(cmd->out), O_WRONLY | O_CREAT);
		else if (!ft_strcmp(*tokens, ">>"))  // if encoutered >>
			err = ft_parsefiletoken(&tokens, &(cmd->out), O_APPEND | O_CREAT);
		else if (!ft_strcmp(*tokens, "<"))  // if encoutered <
			err = ft_parsefiletoken(&tokens, &(cmd->in), O_RDONLY);
		else // encountered an arg
			ft_darrpushback(args, ft_strdup(*tokens));
		if (err)
		{
			ft_darrclear(args);
			return (NULL);
		}
		printf("ENDED READING TOKEN AT %p\n", *tokens);
		tokens++;
		printf("TOKEN IS NOW at %p, its address is %p\n", *tokens, tokens);
	}
	ft_darrpushback(args, NULL);
	cmd->args = args->ptr;
	free(args);
	printf("RETURNING ARGS\n");
	return (args->ptr);
}

t_cmd	*ft_parsecommands(char **tokens)
{
	//tokens = 0;
	int		i;
	t_cmd	*commands;
	int		pipefd[2];

	commands = ft_calloc(g_data.cmds, sizeof(t_cmd));
	i = 0;
	while (i < g_data.cmds)
	{
		commands[i].in = 0;
		commands[i].out = 1;
		commands[i].i = i;
		printf("TOKEN IN ft_parsecommands %p\n", tokens);
		if (!(tokens = ft_extractarguments(&commands[i], tokens))) // error while parsing tokens
		{
			ft_freecomands(commands); // free commands and close FDs
			return (NULL);
		}
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
	char	**tokens;


	// TAKE CARE OF THIS COMMENT BLOCK AND UNCOMMENT IT
	// ! treatment of special characters is needed !
	tokens = ft_tokenize(line);
	int x = 0;

	printf("tokens originally: %p\n", tokens);
	printf("tokens originally (dereferenced): %p\n", *tokens);
	while (tokens[x])
	{
		printf("token%d:|%s| at %p, its address is %p\n", x, tokens[x], tokens[x], &tokens[x]);
		x++;
	}
	printf("token%d:|%s| at %p, its address is %p\n", x, tokens[x], tokens[x], &tokens[x]);
	
	g_data.cmds = ft_cmds(tokens);
	printf("COUNTED CMDS\n");
	if (!(commands = ft_parsecommands(tokens)))
	{
		ft_freematrix(tokens);
		return ;
	}
	g_data.prcs = g_data.cmds - ft_isbuiltin(commands[0].args[0]);
	g_data.family = ft_calloc(g_data.prcs, sizeof(pid_t));
	i = 0;
	j = 0;
	while (i < g_data.cmds)
	{
		// this has to be tested separately
		if (g_data.cmds == 1 && ft_isbuiltin(commands[0].args[0]))
			ft_exec(&commands[i]);
		else
		{
			// spinning out child processes
			g_data.family[j] = fork();
			if (g_data.family[j] == 0) // child process
				ft_exec(&commands[i]); // child process will exit here
			else if (g_data.family[j] < 0)
			{
				perror("minishell");
				break ;
			}
			waitpid(g_data.family[i], &g_data.status, 0);
			if (commands[i].out != 1)
				close(commands[i].out);
			if (commands[i].in != 0 && commands[i].in != 1)
				close(commands[i].in);
			j++;
		}
		i++;
	}
	i = 0;
	// while (i < g_data.prcs)
	// 	waitpid(g_data.family[i++], &g_data.status, 0);
	
	// free stuff
	
	ft_freematrix(tokens);
	printf("FREED TOKENS\n");
	ft_freecomands(commands); // also closes FDs
	printf("FREED COMMANDS\n");
	if (g_data.family)
	{
		free(g_data.family);
		g_data.family = NULL;
	}
	printf("FREED FAMILY\n");
}

int	ft_isbuiltin(char *builtin)
{
	if (ft_convertbuiltin(builtin))
		return (1);
	return (0);
}

int	ft_convertbuiltin(char *builtin)
{
	if (!ft_strcmp(builtin, "echo"))
		return (__echo);
	else if (!ft_strcmp(builtin, "cd"))
		return (__cd);
	else if (!ft_strcmp(builtin, "pwd"))
		return (__pwd);
	else if (!ft_strcmp(builtin, "export"))
		return (__export);
	else if (!ft_strcmp(builtin, "unset"))
		return (__unset);
	else if (!ft_strcmp(builtin, "env"))
		return (__env);
	else if (!ft_strcmp(builtin, "exit"))
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

void	ft_exec(t_cmd *cmd)
{
	int		i;
	char	*temp;
	char	*newpath;
	char	**paths;

	// associating fds
	if (!ft_isbuiltin(cmd->args[0]) || g_data.cmds != 1)
		if (dup2(cmd->in, 0) == -1 || dup2(cmd->out, 1) == -1)
			exit(errno);
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
			if (!ft_strcmp(temp, cmd->heredoc))
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
	exit(1);
}

void	ft_error(char *name, char *desc)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(name, 2);
	ft_putstr_fd(": ", 2);
	ft_putstr_fd(desc, 2);
	ft_putstr_fd("\n", 2);
}
