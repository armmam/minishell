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
 * frees commands and tokens
 */
void	ft_free_commands(t_cmd *cmds, char **tokens)
{
	int	i;

	i = 0;
	while (i < g_data.cmds)
	{
		if (cmds[i].heredoc)
			ft_darrclear(cmds[i].heredoc);
		if (cmds[i].args)
			ft_freematrix(cmds[i].args);
		i++;
	}
	free(cmds);
	if (g_data.family)
	{
		free(g_data.family);
		g_data.family = NULL;
	}
	if (tokens)
	{
		ft_freematrix(tokens);
		tokens = NULL;
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
	t_cmd	*selected;
	pid_t	terminated;

	i = 0;
	while (i < g_data.cmds)
	{
		terminated = waitpid(-1, &g_data.status, 0);
		printf("WEXITSTATUS:%d, WTERMSIG:%d, G_DATA.STATUS:%d\n", WEXITSTATUS(g_data.status), WTERMSIG(g_data.status), g_data.status);
		if (!WTERMSIG(g_data.status))
			g_data.status = WEXITSTATUS(g_data.status);
		else
			g_data.status = WTERMSIG(g_data.status) + 128;
		printf("TERMINATED %d WITH EXIT STATUS %d\n", terminated, g_data.status);
		selected = ft_find_command(terminated, commands);
		if (selected)
		{
			if (selected->out != 1)
			{
				printf("CLOSED %d\n", selected->out);
				close(selected->out);
			}
			if (selected->in != 0)
			{
				printf("CLOSED %d\n", selected->in);
				close(selected->in);
			}
		}
		i++;
	}
	if (g_data.status == 131)
		ft_putstr_fd("Quit: 3\n", 1);
}

void	ft_interpret(char *line)
{
	int		i;
	char	**tokens;

	tokens = ft_tokenize(line);
	if (!tokens)
		return ;
	g_data.cmds = ft_count_commands(tokens);
	g_data.commands = ft_parse_commands(tokens);
	if (!g_data.commands)
	{
		ft_freematrix(tokens);
		return ;
	}
	g_data.family = ft_calloc(g_data.cmds, sizeof(pid_t));
	if (g_data.cmds == 1 && ft_isbuiltin(g_data.commands[0].args[0]))
		ft_exec(&g_data.commands[0]);
	else
	{
		i = 0;
		while (i < g_data.cmds)
		{
			// spinning out child processes
			g_data.family[i] = fork();
			if (parentid == getpid())
				printf("CREATED %d\n", g_data.family[i]);
			if (parentid != getpid())
			{
				dprintf(2, "self%d; parent%d\n", getpid(), getppid());
				dprintf(2, "command %d info at %p: in%d, out%d\n", i, &g_data.commands[i], g_data.commands[i].in, g_data.commands[i].out);
				dprintf(2, "printing cmd's args:\n");
				int iter = 0;
				while (g_data.commands[i].args[iter])
				{
					printf("%i:%s\n", iter, g_data.commands[i].args[iter]);
					iter++;
				}
				printf("\n");
			}
			if (g_data.family[i] == 0) // child process
				ft_exec(&g_data.commands[i]); // child process will exit here
			i++;
		}
	}
	ft_ignore_signals();	// this is done so that...
	if (!(g_data.cmds == 1 && ft_isbuiltin(g_data.commands[0].args[0])))
		ft_block_main_process(g_data.commands);
	ft_define_signals();	// no thing such as "double signal" occurs
	ft_free_commands(g_data.commands, tokens);
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
