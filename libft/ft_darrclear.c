/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_darrclear.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amamian <amamian@student.42yerevan.am>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/27 18:49:21 by amamian           #+#    #+#             */
/*   Updated: 2021/08/19 21:41:01 by amamian          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
 * free dynamic memory allocated for a dynamic matrix
 */
void	ft_darrclear(t_darr *darr)
{
	size_t	i;

	i = 0;
	while (i < darr->len)
		free(darr->ptr[i++]);
	free(darr->ptr);
	free(darr);
}
