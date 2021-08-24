#include "minishell.h"

/*
 * returns 0 upon success, 1 upon failure
 */
int		ft_parseheredoc(char ***tokens, t_cmd *cmd)
{
	(*tokens)++;
	if (*tokens == NULL || ((*tokens)[0][0] == '|' && (*tokens)[0][1] == '\0')) // << is the last token in the command
	{
		ft_error(*(*tokens - sizeof(char *)), "syntax error");
		return (1);
	}
	if (!cmd->heredoc) // if have not encountered any heredocs before
		cmd->heredoc = ft_darrnew(0);
	ft_darrpushback(cmd->heredoc, ft_strdup(**tokens));
	return (0);
}

/*
 * returns 0 upon success, 1 upon failure
 */
int		ft_parsefiletoken(char ***tokens, int *cmd_fd, char dir, int open_flag)
{
	int	fd;

	(*tokens)++;
	if (*tokens == NULL || ((*tokens)[0][0] == '|' && (*tokens)[0][1] == '\0')) // </>>/> is the last token in the command
	{
		ft_error(*(*tokens - 1), "syntax error");
		return (1);
	}
	if ((dir == '>' && *cmd_fd != 1) || (dir == '<' && *cmd_fd != 0)) // if already encoutered </>>/> before for this command
		close(*cmd_fd);
	if ((fd = open(**tokens, open_flag, 0644)) == -1)
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
char	**ft_extract_arguments(t_cmd *cmd, char **tokens)
{
	int		err;
	t_darr	*args;

	// printf("TOKENS RECEIVED %p\n", tokens);
	// printf("TOKENS RECEIVED (dereferenced) %p\n", *tokens);
	args = ft_darrnew(0);
	err = 0;
	while (*tokens && ft_strcmp(*tokens, "|")) // haven't run out of tokens and haven't encountered a pipe
	{
		// printf("TOKENS IN ft_extract_arguments %p\n", tokens);
		// printf("TOKEN:|%s| at %p, its address is %p\n", *tokens, *tokens, tokens);
		if (!ft_strcmp(*tokens, "<<"))  // if encountered <<
			err = ft_parseheredoc(&tokens, cmd);
		else if (!ft_strcmp(*tokens, ">"))  // if encoutered >
			err = ft_parsefiletoken(&tokens, &(cmd->out), '>', O_WRONLY | O_CREAT);
		else if (!ft_strcmp(*tokens, ">>"))  // if encoutered >>
			err = ft_parsefiletoken(&tokens, &(cmd->out), '>', O_APPEND | O_WRONLY | O_CREAT);
		else if (!ft_strcmp(*tokens, "<"))  // if encoutered <
			err = ft_parsefiletoken(&tokens, &(cmd->in), '<', O_RDONLY);
		else // encountered an arg
			ft_darrpushback(args, ft_strdup(*tokens));
		if (err)
		{
			ft_darrclear(args);
			return (NULL);
		}
		// printf("ENDED READING TOKEN AT %p\n", *tokens);
		tokens++;
		// printf("TOKEN IS NOW at %p, its address is %p\n", *tokens, tokens);
	}
	if (*tokens && !ft_strcmp(*tokens, "|") && ft_strcmp(*(tokens + 1), "|")) // if the current one is `|` and the next one is not
		tokens++;
	else if (*tokens && !ft_strcmp(*tokens, "|") && !ft_strcmp(*(tokens + 1), "|")) // if both the current and the next ones are `|`
	{
		ft_error(*tokens, "syntax error");
		return (NULL);
	}
	ft_darrpushback(args, NULL);
	cmd->args = args->ptr;
	free(args);
	// printf("RETURNING TOKENS\n");
	return (tokens);
}

t_cmd	*ft_parse_commands(char **tokens)
{
	int		i;
	t_cmd	*commands;
	int		pipefd[2];

	if (!tokens || !(*tokens))
		return (NULL);
	commands = ft_calloc(g_data.cmds, sizeof(t_cmd));
	i = 0;
	if (!ft_strcmp(tokens[i], "|"))
	{
		ft_error(tokens[i], "syntax error");
		return NULL;
	}
	while (i < g_data.cmds)
	{
		commands[i].in = 0;
		commands[i].out = 1;
		commands[i].i = i;
		if (!(tokens = ft_extract_arguments(&commands[i], tokens))) // error while parsing tokens
		{
			ft_free_commands(commands, tokens); // free commands
			return (NULL);
		}
		if (i)
		{
			pipe(pipefd);
			printf("CREATED A PIPE WITH FD'S %d AND %d\n", pipefd[0], pipefd[1]);
			if (commands[i - 1].out != 1)
				close(pipefd[1]);
			else
				commands[i - 1].out = pipefd[1];
			if (commands[i].in != 0)
				close(pipefd[0]);
			else
				commands[i].in = pipefd[0];
		}
		if (commands[i].heredoc)
		{
			size_t j = 0;
			while (j < commands[i].heredoc->len)
			{
				int	refined = 0;
				char	*heredoc, *temp;
				if (ft_isquoted(commands[i].heredoc->ptr[j], '\''))
				{
					heredoc = commands[i].heredoc->ptr[j] + 1; // if any of the quotes
					heredoc[ft_strlen(heredoc) - 1] = '\0';
					refined = 1; // only if not `'`
				} else
					heredoc = commands[i].heredoc->ptr[j];
				j++;
				while (1)
				{
					temp = readline("> ");
					if (!ft_strcmp(temp, heredoc))
					{
						free(temp);
						break ;
					}
					if (refined)
						temp = ft_refineline(temp);
					ft_putstr_fd(temp, commands[i].in);
					ft_putstr_fd("\n", commands[i].in);
					free(temp);
				}
			}
		}

		i++;
	}
	return (commands);
}