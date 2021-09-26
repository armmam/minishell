#include "minishell.h"

/*
 * returns 0 upon success, 1 upon failure
 */
int		ft_parseheredoc(char ***token, char ***quote, t_cmd *cmd)
{
	char *error;

	(*token)++;
	(*quote)++;
	if (*token == NULL || **token == NULL || ((*token)[0][0] == '|' && (*token)[0][1] == '\0') || !ft_strcmp(**token, "<<")) // << is the last token in the command
	{
		if (*token == NULL || **token == NULL)
			error = "newline";
		else
			error = *(*token - 1);
		ft_error(error, "syntax error");
		g_data.status = 258;
		return (1);
	}
	if (!cmd->heredoc) // if have not encountered any heredocs before
		cmd->heredoc = ft_darrnew(0);
	ft_darrpushback(cmd->heredoc, ft_strdup(**token));
	if (!cmd->refine)
		cmd->refine = ft_darrnew(0);
	ft_darrpushback(cmd->refine, ft_strdup(**quote));
	return (0);
}

/*
 * returns 0 upon success, 1 upon failure
 */
int		ft_parsefiletoken(char ***token, char ***quote, int *cmd_fd, int open_flag)
{
	int	fd;
	char	dir;

	dir = ***token;
	(*token)++;
	(*quote)++;
	if (token == NULL || *token == NULL || **token == NULL || ((*token)[0][0] == '|' && (*token)[0][1] == '\0') ||
	(quote && *quote && **quote && !***quote && (!ft_strcmp(**token, "<<") || !ft_strcmp(**token, ">>") || !ft_strcmp(**token, "<") || !ft_strcmp(**token, ">")))) // </>>/> is the last token in the command OR encountered an unquoted redireciton operator after another one
	{
		ft_error(*(*token - 1), "syntax error");
		g_data.status = 258;
		return (1);
	}
	if ((dir == '>' && *cmd_fd != 1) || (dir == '<' && *cmd_fd != 0)) // if already encoutered </>>/> before for this command
		close(*cmd_fd);
	if ((fd = open(**token, open_flag, 0644)) == -1)
	{
		perror("minishell");
		g_data.status = 1;
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
char	**ft_extract_arguments(t_cmd *cmd, char ***token, char ***quote)
{
	int		err;
	t_darr	*args;

	args = ft_darrnew(0);
	err = 0;
	while (*token && **token && ft_strcmp(**token, "|")) // haven't run out of tokens and haven't encountered a pipe
	{
		if (!ft_strcmp(**token, "<<"))  // if encountered <<
			err = ft_parseheredoc(token, quote, cmd);
		else if (!ft_strcmp(**token, ">"))  // if encoutered >
			err = ft_parsefiletoken(token, quote, &(cmd->out), O_TRUNC | O_WRONLY | O_CREAT);
		else if (!ft_strcmp(**token, ">>"))  // if encoutered >>
			err = ft_parsefiletoken(token, quote, &(cmd->out), O_APPEND | O_WRONLY | O_CREAT);
		else if (!ft_strcmp(**token, "<"))  // if encoutered <
			err = ft_parsefiletoken(token, quote, &(cmd->in), O_RDONLY);
		else // encountered an arg
			ft_darrpushback(args, ft_strdup(**token));
		if (err)
		{
			ft_darrclear(args);
			return (NULL);
		}
		// printf("ENDED READING TOKEN AT %p\n", *tokens);
		(*token)++;
		(*quote)++;
		// printf("TOKEN IS NOW at %p, its address is %p\n", *tokens, tokens);
	}
	if (*token && **token && !ft_strcmp(**token, "|") && (*(*token + 1) != NULL && ft_strcmp(*(*token + 1), "|"))) // if the current one is `|` and the next one is not
	{
		(*token)++;
		(*quote)++;
	}
	else if (*token && **token && !ft_strcmp(**token, "|") && (*(*token + 1) == NULL || !ft_strcmp(*(*token + 1), "|"))) // if both the current and the next ones are `|`
	{
		ft_error(**token, "syntax error");
		ft_darrclear(args);
		g_data.status = 258;
		return (NULL);
	}
	if (args->len > 0)
	{
		ft_darrpushback(args, NULL);
		cmd->args = args->ptr;
	}
	else
		free(args->ptr);
	free(args);
	return (*token);
}

t_cmd	*ft_parse_commands(t_tokens *tokens)
{
	int		i;
	t_cmd	*commands;
	int		pipefd[2];
	char	**token; // current token and quote
	char	**quote;

	if (!tokens || !tokens->tokens || !tokens->tokens->ptr || !*tokens->tokens->ptr)
		return (NULL);
	token = tokens->tokens->ptr;
	quote = tokens->quotes->ptr;
	commands = ft_calloc(g_data.cmds, sizeof(t_cmd));
	printf("ft_parse_commands commands: %p\n", commands);
	i = 0;
	if (!ft_strcmp(token[i], "|"))
	{
		ft_error(token[i], "syntax error");
		g_data.status = 258;
		free(commands);
		return (NULL);
	}
	while (i < g_data.cmds)
	{
		commands[i].in = 0;
		commands[i].out = 1;
		commands[i].i = i;
		if (!(ft_extract_arguments(&commands[i], &token, &quote))) // error while parsing tokens
		{
			ft_free_commands(commands); // free commands
			return (NULL);
		}
		if (i)
		{
			pipe(pipefd);
			//printf("CREATED A PIPE WITH FD'S %d AND %d\n", pipefd[0], pipefd[1]);
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
