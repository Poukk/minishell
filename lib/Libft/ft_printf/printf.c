/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   printf.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexanfe <alexanfe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 14:32:16 by alexanfe          #+#    #+#             */
/*   Updated: 2025/09/17 17:16:44 by alexanfe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	process_format(int fd, const char *fmt, va_list ap)
{
	int	count;

	count = 0;
	if (*fmt == 'c')
		count += ft_putchar(fd, va_arg(ap, int));
	else if (*fmt == 's')
		count += ft_putstr(fd, va_arg(ap, char *));
	else if (*fmt == 'p')
		count += ft_putptr(fd, va_arg(ap, unsigned long));
	else if (*fmt == 'd' || *fmt == 'i')
		count += ft_putnbr(fd, va_arg(ap, int));
	else if (*fmt == 'u')
		count += ft_putunbr(fd, va_arg(ap, unsigned int));
	else if (*fmt == 'x')
		count += ft_puthex(fd, va_arg(ap, unsigned int));
	else if (*fmt == 'X')
		count += ft_putupperhex(fd, va_arg(ap, unsigned int));
	else if (*fmt == '%')
		count += ft_putchar(fd, '%');
	return (count);
}

int	ft_dprintf(int fd, const char *fmt, ...)
{
	va_list	ap;
	int		count;

	va_start(ap, fmt);
	count = 0;
	while (*fmt)
	{
		if (*fmt == '%')
		{
			fmt++;
			count += process_format(fd, fmt, ap);
		}
		else
			count += ft_putchar(fd, *fmt);
		fmt++;
	}
	va_end(ap);
	return (count);
}

int	ft_printf(const char *fmt, ...)
{
	va_list	ap;
	int		count;

	va_start(ap, fmt);
	count = 0;
	while (*fmt)
	{
		if (*fmt == '%')
		{
			fmt++;
			count += process_format(1, fmt, ap);
		}
		else
			count += ft_putchar(1, *fmt);
		fmt++;
	}
	va_end(ap);
	return (count);
}
