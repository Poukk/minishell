/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexanfe <alexanfe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 16:43:33 by alexanfe          #+#    #+#             */
/*   Updated: 2024/10/14 16:56:00 by alexanfe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	char	*mapped_string;
	size_t	i;

	mapped_string = malloc(sizeof(char) * ft_strlen(s) + 1);
	if (!mapped_string)
		return (NULL);
	i = 0;
	while (s[i])
	{
		mapped_string[i] = f(i, s[i]);
		i++;
	}
	mapped_string[i] = '\0';
	return (mapped_string);
}
