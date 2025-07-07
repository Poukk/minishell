/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   printf_number.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexanfe <alexanfe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 14:37:15 by alexanfe          #+#    #+#             */
/*   Updated: 2024/11/07 14:37:15 by alexanfe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ft_printf.h"

int	ft_putnbr(int nbr)
{
	long	num;
	int		count;

	num = nbr;
	count = 0;
	if (num < 0)
	{
		count += ft_putchar('-');
		num = -num;
	}
	if (num >= 10)
		count += ft_putnbr(num / 10);
	count += ft_putchar((num % 10) + '0');
	return (count);
}

int	ft_putunbr(unsigned int nbr)
{
	int		count;

	count = 0;
	if (nbr >= 10)
		count += ft_putunbr(nbr / 10);
	count += ft_putchar((nbr % 10) + '0');
	return (count);
}
