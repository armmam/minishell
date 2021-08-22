#include "minishell.h"

int	ft_count_commands(char **tokens)
{
	int	i;
	int	count;

	if (!tokens || !(*tokens))
		return (0);
	count = 1;
	i = 0;
	// printf("ABOUT TO START COUNTING\n");
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

void	ft_interpret(char *line)
{
	int		i;
	int		j;
	t_cmd	*commands;
	char	**tokens;


	// TAKE CARE OF THIS COMMENT BLOCK AND UNCOMMENT IT
	// ! treatment of special characters is needed !
	tokens = ft_tokenize(line);
	// int x = 0;

	// printf("tokens originally: %p\n", tokens);
	// printf("tokens originally (dereferenced): %p\n", *tokens);
	// while (tokens[x])
	// {
	// 	printf("token%d:|%s| at %p, its address is %p\n", x, tokens[x], tokens[x], &tokens[x]);
	// 	x++;
	// }
	// printf("token%d:|%s| at %p, its address is %p\n", x, tokens[x], tokens[x], &tokens[x]);
	
	g_data.cmds = ft_count_commands(tokens);
	// printf("COUNTED CMDS\n");
	if (!(commands = ft_parse_commands(tokens)))
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
			if (parentid != getpid())
			{
				dprintf(2, "self%d; parent%d\n", getpid(), getppid());
				dprintf(2, "command %d info at %p: in%d, out%d\n", i, &commands[i], commands[i].in, commands[i].out);
				dprintf(2, "printing cmd's args:\n");
				int iter = 0;
				while (commands[i].args[iter])
				{
					printf("%i:%s\n", iter, commands[i].args[iter]);
					iter++;
				}
				printf("\n");
			}
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
	ft_free_commands(commands, tokens);
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
