/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_darrpushback.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amamian <amamian@student.42yerevan.am>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/27 18:13:21 by amamian           #+#    #+#             */
/*   Updated: 2021/08/19 21:40:11 by amamian          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
 * calculate capacity for the new underlying matrix (when the darr->ptr
 * in use cannot fit the new element)
 */
static size_t	cap_calc(size_t cap)
{
	if (cap == 0)
		cap = 1;
	else if (cap < 1024)
		cap *= 2;
	else
		cap += (1048576 / cap + 1);
	return(cap);
}

/*
 * copy first `len` items of `m2` into `m1`
 */
static void		matrix_copy(char **m1, char **m2, size_t len)
{
	size_t		i;

	i = 0;
	while (i < len)
	{
		m1[i] = m2[i];
		i++;
	}
}

/*
 * push an item to the end of a dynamic matrix and expand the underlying matrix
 * if necessary
 */
void			ft_darrpushback(t_darr *darr, char *item)
{
	size_t	new_cap;
	char	**new_ptr;

	if (!darr || !item)
		return ;
	/*
	 * replace the underlying matrix with one with more capacity if darr->ptr
	 * doesn't have enough capacity
	 */
	if (darr->cap - darr->len <= 1)
	{
		if (darr->cap > darr->cap + 1)
			return ;
		new_cap = cap_calc(darr->cap);
		if (!(new_ptr = ft_calloc(new_cap, sizeof(char *))))
			return ;
		matrix_copy(new_ptr, darr->ptr, darr->len);
		free(darr->ptr);
		darr->ptr = new_ptr;
		darr->cap = new_cap;
	}
	/*
	 * push the new item back the dynamic matrix
	 */
	darr->ptr[darr->len] = item;
	darr->len++;
}
