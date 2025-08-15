/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexanfe <alexanfe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 17:21:12 by alexanfe          #+#    #+#             */
/*   Updated: 2024/10/14 17:22:17 by alexanfe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

int	ft_putnbr_fd(int n, int fd)
{
	char	*number;
	int		len;

	number = ft_itoa(n);
	if (!number)
		return (0);
	ft_putstr_fd(number, fd);
	len = ft_strlen(number);
	free(number);
	return (len);
}
