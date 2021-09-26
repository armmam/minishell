#include "minishell.h"

int	ft_count_commands(char **tokens)
{
	int	i;
	int	count;

	if (!tokens || !(*tokens))
		return (0);
	count = 1;
	i = 0;
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
void	ft_free_commands(t_cmd *cmds)
{
	int	i;

	i = 0;
	if (cmds)
	{
		while (i < g_data.cmds)
		{
			if (cmds[i].in != 0 && cmds[i].in != 1)
				close(cmds[i].in);
			if (cmds[i].out != 1 && cmds[i].out != 0)
				close(cmds[i].out);
			if (cmds[i].heredoc)
				ft_darrclear(cmds[i].heredoc);
			if (cmds[i].refine)
				ft_darrclear(cmds[i].refine);
			if (cmds[i].args)
				ft_freematrix(cmds[i].args);
			i++;
		}
		free(cmds);
		cmds = NULL;
	}
	if (g_data.family)
	{
		free(g_data.family);
		g_data.family = NULL;
	}
}

void	ft_free_tokens(t_tokens *tokens)
{
	if (tokens)
	{
		if (tokens->tokens)
			ft_darrclear(tokens->tokens);
		if (tokens->quotes)
			ft_darrclear(tokens->quotes);
		free(tokens);
	}
}

t_cmd	*ft_find_command(pid_t pid, t_cmd *commands)
{
	int	i;

	i = 0;
	while (i < g_data.cmds)
	{
		if (g_data.family[i] == pid)
			return (&commands[i]);
		i++;
	}
	return (NULL);
}

void	ft_block_main_process(t_cmd *commands)
{
	int		i;
	int		signaled;
	int		returned;
	t_cmd	*selected;
	pid_t	terminated;

	signaled = 0;
	i = 0;
	while (i < g_data.cmds)
	{
		terminated = waitpid(-1, &returned, 0);
		if (terminated == g_data.family[g_data.cmds - 1])	// save only the LAST one's
		{
			if (!WTERMSIG(returned))
				g_data.status = WEXITSTATUS(returned);
			else
			{
				signaled = 1;
				g_data.status = WTERMSIG(returned) + 128;
			}
		}
		selected = ft_find_command(terminated, commands);
		if (selected)
		{
			if (selected->out != 1)
				close(selected->out);
			if (selected->in != 0)
				close(selected->in);
		}
		i++;
	}
	// IF there was a signal
	if (signaled)
	{
		if (g_data.status == 131)
			ft_putstr_fd("Quit: 3", 1);
		ft_putstr_fd("\n", 1);		
	}
}

void	ft_interpret(char *line)
{
	int		i;
	t_tokens	*tokens;

	tokens = ft_tokenize(line);
	if (!tokens->tokens)
		return ;
	g_data.cmds = ft_count_commands(tokens->tokens->ptr);
	g_data.commands = ft_parse_commands(tokens);
	if (!g_data.commands || ft_launch_heredoc())
	{
		ft_free_commands(g_data.commands);
		ft_free_tokens(tokens);
		return ;
	}
	g_data.family = ft_calloc(g_data.cmds, sizeof(pid_t));
	if (g_data.cmds == 1 && g_data.commands[0].args && ft_isbuiltin(g_data.commands[0].args[0]))
		ft_exec(&g_data.commands[0]);
	else
	{
		i = 0;
		while (i < g_data.cmds)
		{
			g_data.family[i] = fork();
			if (g_data.family[i] == 0)
			{
				if (!g_data.commands[i].args)
					exit(0);
				ft_default_signals();
				ft_exec(&g_data.commands[i]); // child process will exit here
			} // child process
			i++;
		}
	}
	ft_ignore_signals();	// this is done so that...
	if (!(g_data.cmds == 1 && g_data.commands[0].args && ft_isbuiltin(g_data.commands[0].args[0])))
		ft_block_main_process(g_data.commands);
	ft_define_signals();	// no thing such as "double signal" occurs
	ft_free_commands(g_data.commands);
	ft_free_tokens(tokens);
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

int		ft_isquoted(char *str, char c)
{
	if (str[0] == c && str[ft_strlen(str) - 1] == c)
		return (1);
	return (0);
}

int	ft_error(char *name, char *desc)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(name, 2);
	ft_putstr_fd(": ", 2);
	ft_putstr_fd(desc, 2);
	ft_putstr_fd("\n", 2);
	return (1);
}
