/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexanfe <alexanfe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 18:43:19 by alexanfe          #+#    #+#             */
/*   Updated: 2024/11/07 14:40:23 by alexanfe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# include <stdarg.h>
# include <unistd.h>

int	ft_printf(const char *fmt, ...);
int	ft_dprintf(int fd, const char *fmt, ...);
int	ft_puthex(int fd, unsigned long num);
int	ft_putupperhex(int fd, unsigned long num);
int	ft_putptr(int fd, unsigned long ptr);
int	ft_putnbr(int fd, int nbr);
int	ft_putunbr(int fd, unsigned int nbr);
int	ft_putchar(int fd, char c);
int	ft_putstr(int fd, char *str);
#endif
