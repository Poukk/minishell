/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putstr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexanfe <alexanfe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 17:10:40 by alexanfe          #+#    #+#             */
/*   Updated: 2024/10/14 17:12:12 by alexanfe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

int	ft_putstr_fd(char *s, int fd)
{
	size_t	s_len;
	ssize_t	written;

	if (!s)
		return (0);
	s_len = ft_strlen(s);
	written = write(fd, s, s_len);
	if (written < 0)
		return (0);
	return ((int)written);
}
