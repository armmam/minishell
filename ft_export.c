/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aisraely <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/27 17:23:33 by aisraely          #+#    #+#             */
/*   Updated: 2021/09/27 17:23:34 by aisraely         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	ft_export_variable(t_cmd *cmd, int *ret, int i)
{
	while (cmd->args[i])
	{
		if (!ft_isvaliddeclaration(cmd->args[i]))
		{
			ft_putstr_fd("minishell: export: `", 2);
			ft_putstr_fd(cmd->args[i++], 2);
			ft_putstr_fd("': not a valid identifier\n", 2);
			*ret = 1;
			continue ;
		}
		if (ft_isdefined(cmd->args[i]))
		{
			if (ft_strchr(cmd->args[i], '='))
				ft_darrerase(g_data.env, ft_getenv_full(cmd->args[i]));
			else
			{
				*ret = 0;
				i++;
				continue ;
			}
		}
		ft_darrpushback(g_data.env, ft_strdup(cmd->args[i]));
		*ret = 0;
		i++;
	}
}

int	ft_export(t_cmd *cmd)
{
	int	ret;

	ret = 0;
	if (ft_matrixlen(cmd->args) == 1)
		ft_print_environment(cmd->out);
	else
		ft_export_variable(cmd, &ret, 1);
	return (ret);
}
