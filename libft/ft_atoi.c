/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aisraely <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/16 20:22:12 by amamian           #+#    #+#             */
/*   Updated: 2021/09/27 16:39:25 by aisraely         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_isspace(int c)
{
	unsigned char	ch;

	ch = (unsigned char)c;
	if (ch == '\t' || ch == '\n' || ch == '\v'
		|| ch == '\f' || ch == '\r' || ch == ' ')
		return (true);
	return (false);
}

int	ft_atoi(const char *str)
{
	unsigned long long	ret;
	bool				neg;

	ret = 0;
	while (ft_isspace(*str))
		str++;
	neg = (*str == '-');
	if (neg || *str == '+')
		str++;
	ret = 0;
	while (ft_isdigit(*str))
		ret = ret * 10 + (*str++ - '0');
	if (ret > LONG_MAX && !neg)
		return (-1);
	if (ret > LONG_MAX - 1 && neg)
		return (0);
	if (neg)
		return (-ret);
	return (ret);
}
