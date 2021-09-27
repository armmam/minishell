/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aisraely <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/27 17:24:28 by aisraely          #+#    #+#             */
/*   Updated: 2021/09/27 17:24:30 by aisraely         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	ft_unset_variable(t_cmd *cmd, int *ret, int i)
{
	while (cmd->args[i])
	{
		if (!ft_isvalididentifier(cmd->args[i]))
		{
			ft_putstr_fd("minishell: unset: `", 2);
			ft_putstr_fd(cmd->args[i++], 2);
			ft_putstr_fd("': not a valid identifier\n", 2);
			*ret = 1;
			continue ;
		}
		if (ft_isdefined(cmd->args[i]))
		{
			ft_darrerase(g_data.env, ft_getenv_full(cmd->args[i]));
			*ret = 0;
		}
		i++;
	}
}

int	ft_unset(t_cmd *cmd)
{
	int	ret;

	ret = 0;
	if (ft_matrixlen(cmd->args) != 1)
		ft_unset_variable(cmd, &ret, 1);
	return (ret);
}
