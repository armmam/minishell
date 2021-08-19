/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_dmtxclear.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amamian <amamian@student.42yerevan.am>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/27 18:49:21 by amamian           #+#    #+#             */
/*   Updated: 2021/08/19 14:06:22 by amamian          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
 * free dynamic memory allocated for a dynamic matrix
 */
void	ft_dmtxclear(t_dmtx *dmtx)
{
	size_t	i;

	i = 0;
	while (i < dmtx->len)
		free(dmtx->ptr[i++]);
	free(dmtx->ptr);
	free(dmtx);
}
