#include "minishell.h"

int	main(int argc, char **argv, char **environ)
{
	char	*line;

	(void)argc;
	(void)argv;
	ft_inheritenviron(environ);
	g_data.status = 1;
	while (1)
	{
		line = readline("minishell$ ");
		ft_interpret(line);
		free(line);
	}
	// ft_terminate();
	return (EXIT_SUCCESS);
}
