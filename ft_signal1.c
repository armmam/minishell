/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_signal1.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aisraely <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/27 16:15:30 by aisraely          #+#    #+#             */
/*   Updated: 2021/09/27 16:15:32 by aisraely         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_define_signals(void)
{
	signal(SIGINT, ft_reprompt);
	signal(SIGQUIT, ft_do_nothing);
}

void	ft_default_signals(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
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
