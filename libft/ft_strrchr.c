/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amamian <amamian@student.42yerevan.am>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/14 14:44:22 by amamian           #+#    #+#             */
/*   Updated: 2021/04/20 16:25:28 by amamian          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	char *ret;

	ret = NULL;
	while (true)
	{
		if (*s == (char)c)
			ret = (char *)s;
		if (*s == '\0')
			return (ret);
		s++;
	}
}
