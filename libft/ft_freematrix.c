/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_freematrix.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aisraely <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/27 14:18:28 by aisraely          #+#    #+#             */
/*   Updated: 2021/07/27 14:18:31 by aisraely         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_freematrix(char ***matrix)
{
	int	i;

	i = ft_matrixlen(*matrix) - 1;
	while (i >= 0)
		free((*matrix)[i--]);
	free(*matrix);
}
