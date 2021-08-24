#include "minishell.h"

void	ft_reprompt(int sig)
{
	(void)sig;
	ft_suppress_output();
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	ft_do_nothing(int sig)
{
	(void)sig;
	ft_suppress_output();
}

void	ft_configure_terminal(void)
{
	if (tcgetattr(0, &g_data.settings))
		perror("minishell: tcsetattr");
	signal(SIGINT, ft_reprompt);
	signal(SIGQUIT, ft_do_nothing);
}

void	ft_suppress_output(void)
{
	struct termios	new_settings;

	new_settings = g_data.settings;
	new_settings.c_lflag &= ~ECHOCTL;
	if (tcsetattr(0, 0, &new_settings))
		perror("minishell: tcsetattr");
}

void	ft_reset_terminal(void)
{
	int	ret;

	ret = tcsetattr(0, 0, &g_data.settings);
	if (ret)
		perror("minishell: tcsetattr");
}