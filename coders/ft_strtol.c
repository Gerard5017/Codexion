/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtol.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emarette <emarette@student.42lehavre.fr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/01 03:19:58 by emarette          #+#    #+#             */
/*   Updated: 2026/03/02 07:31:58 by emarette         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	ft_isdigit(char c)
{
	return ('0' <= c && c <= '9');
}

int	ft_isspace(char c)
{
	return (('\t' <= c && c <= '\r') || c == ' ');
}

long	get_nb(const char **str, int sign)
{
	unsigned long	res;

	res = 0;
	while (ft_isdigit(**str))
	{
		res *= 10;
		res += **str - '0';
		if (LONG_MAX < res - (sign == -1))
		{
			errno = ERANGE;
			if (sign < 0)
				return (LONG_MIN);
			return (LONG_MAX);
		}
		(*str)++;
	}
	return (res * sign);
}

long	ft_strtol(const char *str, char **endptr)
{
	long	res;
	long	sign;

	if (endptr)
		*endptr = (char *)str;
	while (ft_isspace(*str))
		str++;
	sign = 1;
	if (*str == '+' || *str == '-')
	{
		if (*str == '-')
			sign = -1;
		str++;
	}
	if (!ft_isdigit(*str))
		return (0);
	res = get_nb(&str, sign);
	if (endptr)
		*endptr = (char *)str;
	return (res);
}
