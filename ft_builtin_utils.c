/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_builtins_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aisraely <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/27 17:18:46 by aisraely          #+#    #+#             */
/*   Updated: 2021/09/27 17:18:47 by aisraely         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * takes in a declaration; returns 1 if it is already defined,
 * 0 otherwise 
 */
char	*ft_isdefined(char *decl)
{
	char	*ret;
	char	*name;

	name = ft_separate_identifier(decl);
	ret = ft_getenv(name);
	free(name);
	return (ret);
}

/*
 * goes through a string and checks whether
 * it's a valid identifier. returns 1 if it is,
 * 0 otherwise
 */
int	ft_isvalididentifier(const char *name)
{
	int	i;

	i = 0;
	if (!name)
		return (0);
	if (!(ft_isalpha(name[i]) || name[i] == '_') || name[i] == '=')
		return (0);
	i++;
	while (name[i])
	{
		if (!(ft_isalnum(name[i]) || name[i] == '_') || name[i] == '=')
			return (0);
		i++;
	}
	return (1);
}

/*
 * checks whether a declaration's varname is valid
 */
int	ft_isvaliddeclaration(char *decl)
{
	int		ret;
	char	*name;

	name = ft_separate_identifier(decl);
	ret = ft_isvalididentifier(name);
	free(name);
	return (ret);
}

/*
 * takes a declaration string, separates
 * the identifier before the first '='
 * and returns it. must be freed later
 */
char	*ft_separate_identifier(char *decl)
{
	int		i;
	char	*name;

	i = 0;
	while (decl[i] && decl[i] != '=')
		i++;
	name = ft_calloc(i + 1, sizeof(char));
	i = 0;
	while (decl[i] && decl[i] != '=')
	{
		name[i] = decl[i];
		i++;
	}
	return (name);
}

void	ft_print_environment(int fd)
{
	int	i;
	int	j;

	i = 0;
	while (g_data.env->ptr[i])
	{
		ft_putstr_fd("declare -x ", fd);
		j = 0;
		while (g_data.env->ptr[i][j])
		{
			ft_putchar_fd(g_data.env->ptr[i][j], fd);
			if (g_data.env->ptr[i][j++] == '=')
				break ;
		}
		if (g_data.env->ptr[i][j - 1] == '=')
		{
			ft_putstr_fd("\"", fd);
			while (g_data.env->ptr[i][j])
				ft_putchar_fd(g_data.env->ptr[i][j++], fd);
			ft_putstr_fd("\"", fd);
		}
		ft_putstr_fd("\n", fd);
		i++;
	}
}
