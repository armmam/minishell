/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aisraely <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/27 16:14:39 by aisraely          #+#    #+#             */
/*   Updated: 2021/09/27 16:14:41 by aisraely         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(int argc, char **argv, char **environ)
{
	char	*line;

	(void)argc;
	(void)argv;
	ft_inherit_environment(environ);
	ft_define_signals();
	g_data.status = 0;
	while (1)
	{
		line = readline("minishell$ ");
		if (!line)
		{
			ft_putstr_fd("exit\n", 1);
			return (EXIT_SUCCESS);
		}
		ft_interpret(line);
		if (ft_strcmp(line, ""))
			add_history(line);
		free(line);
	}
	return (EXIT_SUCCESS);
}
