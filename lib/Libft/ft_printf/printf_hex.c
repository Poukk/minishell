/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   printf_hex.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexanfe <alexanfe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 14:32:52 by alexanfe          #+#    #+#             */
/*   Updated: 2025/09/17 17:16:38 by alexanfe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_puthex(int fd, unsigned long num)
{
	int		count;
	char	*hex;

	count = 0;
	hex = "0123456789abcdef";
	if (num >= 16)
		count += ft_puthex(fd, num / 16);
	count += write(fd, &hex[num % 16], 1);
	return (count);
}

int	ft_putupperhex(int fd, unsigned long num)
{
	int		count;
	char	*hex;

	count = 0;
	hex = "0123456789ABCDEF";
	if (num >= 16)
		count += ft_putupperhex(fd, num / 16);
	count += write(fd, &hex[num % 16], 1);
	return (count);
}

int	ft_putptr(int fd, unsigned long ptr)
{
	int	count;

	count = 0;
	if (ptr == 0)
		return (write(fd, "(nil)", 5));
	count += write(fd, "0x", 2);
	count += ft_puthex(fd, ptr);
	return (count);
}
