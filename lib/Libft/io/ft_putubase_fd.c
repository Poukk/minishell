/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putubase_fd.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elvictor <elvictor@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/15 19:58:03 by elvictor          #+#    #+#             */
/*   Updated: 2025/08/15 20:14:29 by elvictor         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	check_base_and_get_length(char *base)
{
	int	base_len;
	int	i;

	base_len = 0;
	while (base[base_len])
	{
		if (base[base_len] == '+' || base[base_len] == '-')
			return (0);
		i = base_len + 1;
		while (base[i])
		{
			if (base[base_len] == base[i])
				return (0);
			i++;
		}
		base_len++;
	}
	if (base_len < 2)
		return (0);
	return (base_len);
}

int	ft_putubase_fd(unsigned int nbr, char *base, int fd)
{
	int		counter;
	int		baselen;

	counter = 0;
	baselen = check_base_and_get_length(base);
	if (baselen == 0)
		return (0);
	if (nbr >= (unsigned int)baselen)
		counter += ft_putubase_fd(nbr / baselen, base, fd);
	counter += ft_putchar_fd(base[nbr % baselen], fd);
	return (counter);
}
