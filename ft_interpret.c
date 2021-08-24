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
			free(cmds[i].heredoc);
		// printf("FREED HEREDOC\n");
		if (cmds[i].args)
			ft_freematrix(cmds[i].args);
		// printf("FREED ARGS\n");
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
		printf("TERMINATED %d\n", terminated);
		selected = ft_find_command(terminated, commands);
		printf("CMD IS AT %p\n", selected);
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
}

void	ft_interpret(char *line)
{
	int		i;
	char	**tokens;

	tokens = ft_tokenize(line);
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
	if (!(g_data.cmds == 1 && ft_isbuiltin(g_data.commands[0].args[0])))
		ft_block_main_process(g_data.commands);
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

void	ft_error(char *name, char *desc)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(name, 2);
	ft_putstr_fd(": ", 2);
	ft_putstr_fd(desc, 2);
	ft_putstr_fd("\n", 2);
}
