/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   printf.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexanfe <alexanfe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 14:32:16 by alexanfe          #+#    #+#             */
/*   Updated: 2024/11/07 14:51:15 by alexanfe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ft_printf.h"

int	process_format(const char *fmt, va_list ap)
{
	int	count;

	count = 0;
	if (*fmt == 'c')
		count += ft_putchar(va_arg(ap, int));
	else if (*fmt == 's')
		count += ft_putstr(va_arg(ap, char *));
	else if (*fmt == 'p')
		count += ft_putptr(va_arg(ap, unsigned long));
	else if (*fmt == 'd' || *fmt == 'i')
		count += ft_putnbr(va_arg(ap, int));
	else if (*fmt == 'u')
		count += ft_putunbr(va_arg(ap, unsigned int));
	else if (*fmt == 'x')
		count += ft_puthex(va_arg(ap, unsigned int));
	else if (*fmt == 'X')
		count += ft_putupperhex(va_arg(ap, unsigned int));
	else if (*fmt == '%')
		count += ft_putchar('%');
	fmt++;
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
			count += process_format(fmt, ap);
		}
		else
			count += ft_putchar(*fmt);
		fmt++;
	}
	va_end(ap);
	return (count);
}
