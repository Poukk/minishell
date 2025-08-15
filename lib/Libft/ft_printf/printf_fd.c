/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   printf_fd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elvictor <elvictor@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 16:33:18 by elvictor          #+#    #+#             */
/*   Updated: 2025/08/15 20:26:08 by elvictor         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ft_printf.h"
#include "../include/libft.h"

static int	ft_print_format(const int fd, const char specifier, va_list args)
{
	if (specifier == 'c')
		return (ft_putchar_fd(va_arg(args, int), fd));
	else if (specifier == 's')
		return (ft_putstr_fd(va_arg(args, char *), fd));
	else if (specifier == 'd' || specifier == 'i')
		return (ft_putnbr_fd(va_arg(args, int), fd));
	else if (specifier == '%')
		return (ft_putchar_fd('%', fd));
	else if (specifier == 'p')
		return (ft_putptr_fd((unsigned long)va_arg(args, void *), fd));
	else if (specifier == 'u')
		return (ft_putubase_fd(va_arg(args, unsigned int), "0123456789", fd));
	else if (specifier == 'x')
		return (ft_putubase_fd(va_arg(args, unsigned int),
				"0123456789abcdef", fd));
	else if (specifier == 'X')
		return (ft_putubase_fd(va_arg(args, unsigned int),
				"0123456789ABCDEF", fd));
	else
		return (0);
}

int	ft_printf_fd(const int fd, const char *str, ...)
{
	va_list	args;
	int		i;
	int		printed;

	if (fd < 0)
		return (-1);
	va_start(args, str);
	i = 0;
	printed = 0;
	while (str[i])
	{
		if (str[i] == '%' && str[i + 1])
		{
			printed += ft_print_format(fd, str[i + 1], args);
			i += 2;
		}
		else
			printed += ft_putchar_fd(str[i++], fd);
	}
	va_end(args);
	return (printed);
}

