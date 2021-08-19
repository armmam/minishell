/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_darrerase.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amamian <amamian@student.42yerevan.am>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/18 17:43:21 by amamian           #+#    #+#             */
/*   Updated: 2021/08/19 22:07:45 by amamian          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
 * erase an item from dynamic array
 * modifies `len` and leaves `cap` unchanged
 */
void	ft_darrerase(t_darr *darr, char *item)
{
	size_t	i, j, new_len; // i -- index of the resulting mtx, j -- index of the given mtx

	if (!darr || !item || !*item)
		return ;
	i = 0;
	j = 0;
	/*
	 * iterate over the underlying array while freeing and skipping all `item`s (j)
	 * and move the strings that are not equal to `item` to their corresponding position (i)
	 */
	while (j < darr->len)
	{
		if (!ft_strcmp(darr->ptr[j], item))
		{
			while (!ft_strcmp(darr->ptr[j], item))
			{
				free(darr->ptr[j]);
				j++;
			}
		}
		darr->ptr[i] = darr->ptr[j];
		j++;
		i++;
	}
	/*
	 * remember the new `len` and fill the rest of the remaining (between new `len` and old `len) positions with NULL
	 */
	new_len = i; // don't count the first NULL after the last valid element in the new mtx as a valid element
	while (i < darr->len)
		darr->ptr[i++] = NULL;
	darr->len = new_len;
}
