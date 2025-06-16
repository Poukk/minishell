/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexanfe <alexanfe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 16:02:08 by alexanfe          #+#    #+#             */
/*   Updated: 2024/10/14 16:04:13 by alexanfe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	num_length(int n)
{
	int	len;

	len = 0;
	if (n <= 0)
		len = 1;
	while (n)
	{
		len++;
		n /= 10;
	}
	return (len);
}

static void	fill_string(char *str, int n, int len)
{
	int				is_negative;
	unsigned int	num;

	is_negative = (n < 0);
	num = n;
	if (is_negative)
	{
		num = -n;
		str[0] = '-';
	}
	str[len] = '\0';
	while (len-- > is_negative)
	{
		str[len] = '0' + (num % 10);
		num /= 10;
	}
}

char	*ft_itoa(int n)
{
	int		len;
	char	*str;

	len = num_length(n);
	str = (char *)malloc(len + 1);
	if (!str)
		return (NULL);
	fill_string(str, n, len);
	return (str);
}
