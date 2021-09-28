/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_refineline.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amamian <amamian@student.42yerevan.am>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/27 16:20:50 by amamian           #+#    #+#             */
/*   Updated: 2021/09/28 15:40:12 by amamian          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * takes a string and substitutes every environ variable encountered
 * frees the original string, allocates memory
 */
char	*ft_refineline(char *line)
{
	t_refine	r;
	int			cont;

	if (!line)
		return (NULL);
	r.ptr = line;
	r.ptr = ft_strchr(r.ptr, '$');
	while (r.ptr && *(r.ptr))
	{
		cont = ft_process_dollarsign(&r, line);
		if (cont)
			continue ;
		if (r.val)
			r.ptr = ft_strjoin3(r.prefix, r.val, r.postfix);
		else
			r.ptr = ft_strjoin(r.prefix, r.postfix);
		free(line);
		line = r.ptr;
		r.ptr += ft_strlen(r.prefix) + ft_strlen(r.val);
		free(r.prefix);
		free(r.postfix);
		free(r.val);
		r.ptr = ft_strchr(r.ptr, '$');
	}
	return (line);
}
