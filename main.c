#include "minishell.h"

void	ft_reprompt(int arg)
{
	(void)arg;
	ft_putstr_fd("\nminishell$ ", 1);
}


int	main(int argc, char **argv, char **environ)
{
	char			*line;

	(void)argc;
	(void)argv;
	ft_inheritenviron(environ);
	// signal(SIGINT, &ft_reprompt);
	parentid = getpid();
	printf("SHELL PROCESS PID IS %d\n", getpid());
	g_data.status = 0;
	while (1)
	{
		line = readline("minishell$ ");
		ft_interpret(line);
		add_history(line);
		free(line);
	}
	return (EXIT_SUCCESS);
}
