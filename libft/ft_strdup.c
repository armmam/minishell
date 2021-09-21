/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aisraely <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/18 18:18:47 by amamian           #+#    #+#             */
/*   Updated: 2021/09/21 19:43:51 by aisraely         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *s1)
{
	char	*s2;
	size_t	len;

	len = ft_strlen(s1);
	if (!(s2 = (char *)malloc((len + 1) * sizeof(char))))
		return (NULL);
	printf("ft_strdup s2: %p\n", s2);
	ft_strlcpy(s2, s1, len + 1);
	return (s2);
}
