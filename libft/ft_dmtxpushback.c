/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_dmtxpushback.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amamian <amamian@student.42yerevan.am>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/27 18:13:21 by amamian           #+#    #+#             */
/*   Updated: 2021/08/04 18:49:57 by amamian          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
 * calculate capacity for the new underlying matrix (when the dmtx->ptr
 * in use cannot fit the new element)
 */
static size_t	cap_calc(size_t cap)
{
	return(cap < 1024 ? cap * 2 : cap + (1048576 / cap + 1));
}

/*
 * push an item to the end of a dynamic matrix and expand the underlying matrix
 * if necessary
 */
void			ft_dmtxpushback(t_dmtx *dmtx, char *item)
{
	size_t	new_cap;
	char	*new_ptr;

	/*
	 * replace the underlying matrix with one with more capacity if dmtx->ptr
	 * doesn't have enough capacity
	 */
	if (dmtx->cap - dmtx->len == 1)
	{
		if (dmtx->cap > dmtx->cap + 1)
			return ;
		new_cap = cap_calc(dmtx->cap);
		if (!(new_ptr = ft_calloc(new_cap, sizeof(char *))))
			return ;
		ft_memmove(new_ptr, dmtx->ptr, dmtx->len);
		free(dmtx->ptr);
		dmtx->ptr = new_ptr;
		dmtx->cap = new_cap;
	}
	/*
	 * push the new item back the dynamic matrix
	 */
	dmtx->ptr[dmtx->len] = item;
	dmtx->len++;
}
