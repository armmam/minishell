/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_signal2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aisraely <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/27 16:15:10 by aisraely          #+#    #+#             */
/*   Updated: 2021/09/27 16:15:12 by aisraely         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

void	ft_get_interrupted(int sig)
{
	(void)sig;
	ft_suppress_output();
	rl_on_new_line();
	rl_redisplay();
	exit(1);
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
