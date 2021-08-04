/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_dmtxnew.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amamian <amamian@student.42yerevan.am>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/27 18:54:21 by amamian           #+#    #+#             */
/*   Updated: 2021/08/04 18:58:08 by amamian          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
 * initialize a new and empty dynamic matrix with a given capacity
 */
t_dmtx	*ft_dmtxnew(size_t cap)
{
	t_dmtx	*dmtx;
	char	**ptr;

	if (!(dmtx = ft_calloc(1, sizeof(t_dmtx))))
		return (NULL);
	if (!(ptr = ft_calloc(cap, sizeof(char *))))
		return (NULL);
	dmtx->ptr = ptr;
	dmtx->len = 0;
	dmtx->cap = cap;
	return (dmtx);
}
