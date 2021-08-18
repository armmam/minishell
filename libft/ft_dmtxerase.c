/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_dmtxerase.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amamian <amamian@student.42yerevan.am>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/18 17:43:21 by amamian           #+#    #+#             */
/*   Updated: 2021/08/18 19:26:04 by amamian          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
 * erase an item from dynamic matrix
 * modifies `len` and leaves `cap` unchanged
 */
void	ft_dmtxerase(t_dmtx *dmtx, char *item)
{
	size_t	i, j, new_len; // i -- index of the resulting mtx, j -- index of the given mtx

	if (!dmtx || !item || !*item)
		return ;
	i = 0;
	j = 0;
	/*
	 * iterate over the underlying matrix while freeing and skipping all `item`s (j)
	 * and move the strings that are not equal to `item` to their corresponding position (i)
	 */
	while (j < dmtx->len)
	{
		if (!ft_strcmp(dmtx->ptr[j], item))
		{
			while (!ft_strcmp(dmtx->ptr[j], item))
			{
				free(dmtx->ptr[j]);
				j++;
			}
		}
		dmtx->ptr[i] = dmtx->ptr[j];
		j++;
		i++;
	}
	/*
	 * remember the new `len` and fill the rest of the remaining (between new `len` and old `len) positions with NULL
	 */
	new_len = i; // don't count the first NULL after the last valid element in the new mtx as a valid element
	while (i < dmtx->len)
		dmtx->ptr[i++] = NULL;
	dmtx->len = new_len;
}
