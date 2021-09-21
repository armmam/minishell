/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aisraely <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/18 18:18:51 by amamian           #+#    #+#             */
/*   Updated: 2021/09/21 19:44:41 by aisraely         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	size_t	sublen;
	char	*ret;

	if (!s)
		return (NULL);
	if (ft_strlen(s) <= start)
		return (ft_strdup(""));
	sublen = ft_strlen(s + start);
	len = sublen < len ? sublen : len;
	if (!(ret = malloc((len + 1) * sizeof(char))))
		return (NULL);
	printf("ft_substr: ret: %p\n", ret);
	ft_strlcpy(ret, s + start, len + 1);
	return (ret);
}
