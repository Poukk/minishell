/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putptr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elvictor <elvictor@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/15 15:58:27 by elvictor          #+#    #+#             */
/*   Updated: 2025/08/15 16:44:41 by elvictor         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	print_pointer(unsigned long nbr, int fd)
{
	int		counter;
	char	*base;

	counter = 0;
	base = "0123456789abcdef";
	if (nbr >= 16)
		counter += print_pointer(nbr / 16, fd);
	counter += ft_putchar_fd(base[nbr % 16], fd);
	return (counter);
}

int	ft_putptr_fd(unsigned long nbr, int fd)
{
	int	counter;

	counter = 0;
	if (nbr == 0)
		counter += write(fd, "(nil)", 5);
	else
	{
		counter += write(fd, "0x", 2);
		counter += print_pointer(nbr, fd);
	}
	return (counter);
}
