/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_darrnew.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amamian <amamian@student.42yerevan.am>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/27 18:54:21 by amamian           #+#    #+#             */
/*   Updated: 2021/08/19 21:41:12 by amamian          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
 * initialize a new and empty dynamic matrix with a given capacity
 */
t_darr	*ft_darrnew(size_t cap)
{
	t_darr	*darr;
	char	**ptr;

	if (!(darr = ft_calloc(1, sizeof(t_darr))))
		return (NULL);
	if (!(ptr = ft_calloc(cap, sizeof(char *))))
		return (NULL);
	darr->ptr = ptr;
	darr->len = 0;
	darr->cap = cap;
	return (darr);
}
