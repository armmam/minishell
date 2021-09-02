#include "minishell.h"

void	ft_define_signals(void)
{
	signal(SIGINT, ft_reprompt);
	signal(SIGQUIT, ft_do_nothing);
}

void	ft_ignore_signals(void)
{
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
}

void	ft_heredoc_signals(void)
{
	signal(SIGINT, ft_get_interrupted);
	signal(SIGQUIT, ft_do_nothing);
}
