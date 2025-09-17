/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   printf_char.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexanfe <alexanfe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 14:39:09 by alexanfe          #+#    #+#             */
/*   Updated: 2025/09/17 17:16:41 by alexanfe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_putchar(int fd, char c)
{
	write(fd, &c, 1);
	return (1);
}

int	ft_putstr(int fd, char *str)
{
	int	i;

	i = 0;
	if (!str)
		return (ft_putstr(fd, "(null)"));
	while (str[i])
	{
		write(fd, &str[i], 1);
		i++;
	}
	return (i);
}
