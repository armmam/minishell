/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aisraely <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/27 17:22:30 by aisraely          #+#    #+#             */
/*   Updated: 2021/09/27 17:22:31 by aisraely         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_env(t_cmd *cmd)
{
	int	i;
	int	j;

	i = 0;
	while (g_data.env->ptr[i])
	{
		j = 0;
		while (g_data.env->ptr[i][j] && g_data.env->ptr[i][j] != '=')
			j++;
		if (g_data.env->ptr[i][j])
		{
			ft_putstr_fd(g_data.env->ptr[i], cmd->out);
			ft_putstr_fd("\n", cmd->out);
		}
		i++;
	}
	return (0);
}
