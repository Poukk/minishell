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

int	vft_printf(const char *fmt, va_list ap);
int	ft_printf(const char *fmt, ...);
int	ft_puthex(unsigned long num);
int	ft_putupperhex(unsigned long num);
int	ft_putptr(unsigned long ptr);
int	ft_putnbr(int nbr);
int	ft_putunbr(unsigned int nbr);
int	ft_putchar(char c);
int	ft_putstr(char *str);
#endif
