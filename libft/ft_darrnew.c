/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_darrnew.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aisraely <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/27 18:54:21 by amamian           #+#    #+#             */
/*   Updated: 2021/09/21 19:43:12 by aisraely         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
 * initialize a new and empty dynamic array with a given capacity
 */
t_darr	*ft_darrnew(size_t cap)
{
	t_darr	*darr;
	char	**ptr;

	if (!(darr = ft_calloc(1, sizeof(t_darr))))
		return (NULL);
	if (!(ptr = ft_calloc(cap, sizeof(char *))))
		return (NULL);
	printf("ft_darrnew darr: %p ptr: %p\n", darr, ptr);
	darr->ptr = ptr;
	darr->len = 0;
	darr->cap = cap;
	return (darr);
}
