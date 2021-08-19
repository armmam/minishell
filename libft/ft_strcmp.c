/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amamian <amamian@student.42yerevan.am>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/18 18:09:43 by aisraely          #+#    #+#             */
/*   Updated: 2021/08/18 18:13:31 by amamian          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_cmpchar(char c1, char c2)
{
	return ((unsigned char)c1 - (unsigned char)c2);
}

int	ft_strcmp(const char *s1, const char *s2)
{
	size_t	i;

	i = 0;
	while (*(s1 + i) && *(s2 + i))
	{
		if (ft_cmpchar(*(s1 + i), *(s2 + i)))
			return ((unsigned char)*(s1 + i) - (unsigned char)*(s2 + i));
		i++;
	}
	return (ft_cmpchar(*(s1 + i), *(s2 + i)));
}
