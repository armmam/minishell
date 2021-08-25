#include "minishell.h"

int	main(int argc, char **argv, char **environ)
{
	char	*line;

	(void)argc;
	(void)argv;
	ft_inherit_environment(environ);
	ft_define_signals();
	//
	parentid = getpid();
	printf("SHELL PROCESS PID IS %d\n", getpid());
	//
	g_data.status = 0;
	while (1)
	{
		line = readline("minishell$ ");
		if (!line)
		{
			ft_putstr_fd("\b \bexit\n", 1);
			return (EXIT_SUCCESS);
		}
		ft_interpret(line);
		add_history(line);
		free(line);
	}
	return (EXIT_SUCCESS);
}
