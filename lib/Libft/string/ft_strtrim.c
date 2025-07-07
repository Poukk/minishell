/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexanfe <alexanfe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/13 10:13:47 by alexanfe          #+#    #+#             */
/*   Updated: 2024/10/13 14:38:00 by alexanfe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	is_in_set(char c, const char *set)
{
	while (*set)
		if (c == *set++)
			return (1);
	return (0);
}

char	*ft_strtrim(const char *s1, const char *set)
{
	size_t	start;
	size_t	end;
	size_t	trim_len;
	char	*trimmed;

	start = 0;
	end = ft_strlen(s1);
	if (!s1 || !set)
		return (NULL);
	while (s1[start] && is_in_set(s1[start], set))
		start++;
	if (start < end)
	{
		while (end > start && is_in_set(s1[end - 1], set))
			end--;
	}
	trim_len = end - start;
	trimmed = malloc(trim_len + 1);
	if (!trimmed)
		return (NULL);
	ft_memcpy(trimmed, s1 + start, trim_len);
	trimmed[trim_len] = '\0';
	return (trimmed);
}
