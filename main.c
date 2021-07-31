#include "minishell.h"

void	ft_reprompt(int arg)
{
	(void)arg;
	ft_putstr_fd("\nminishell$ ", 1);
}


int	main(int argc, char **argv, char **environ)
{
	char			*line;
	struct termios	settings;

	(void)argc;
	(void)argv;
	ft_inheritenviron(environ);
	signal(SIGINT, &ft_reprompt);
	g_data.status = 0;
	while (1)
	{
		line = readline("minishell$ ");
		//ft_interpret(line);

		// THIS IS TO EXIT THE SHELL UNTIL PARSING AND TOKENIZATION ARE DONE
		if (ft_strlen(line) == 4 && !ft_strncmp(line, "exit", 4))
			exit(0);
		//
		add_history(line);
		free(line);
	}
	return (EXIT_SUCCESS);
}
