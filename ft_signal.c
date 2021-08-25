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
	rl_on_new_line();
	rl_redisplay();
}

void	ft_define_signals(void)
{
	signal(SIGINT, ft_reprompt);
	signal(SIGQUIT, ft_do_nothing);
}

// void	ft_default_signals(void)
// {
// 	signal(SIGINT, SIG_DFL);
// 	signal(SIGQUIT, SIG_DFL);
// }

void	ft_ignore_signals(void)
{
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
}

void	ft_suppress_output(void)
{
	struct termios	new_settings;

	if (tcgetattr(0, &new_settings))
		perror("minishell: tcsetattr");
	new_settings.c_lflag &= ~ECHOCTL;
	if (tcsetattr(0, 0, &new_settings))
		perror("minishell: tcsetattr");
}
