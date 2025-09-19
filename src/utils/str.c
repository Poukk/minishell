/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexanfe <alexanfe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/19 16:32:00 by alexanfe          #+#    #+#             */
/*   Updated: 2025/09/19 16:49:35 by alexanfe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*concatenate_strings(t_gc *gc, const char *str1, const char *str2)
{
	char	*result;
	size_t	len1;
	size_t	len2;

	if (!str1 && !str2)
		return (NULL);
	if (!str1)
		return (ft_strdup_gc(gc, str2));
	if (!str2)
		return (ft_strdup_gc(gc, str1));
	len1 = ft_strlen(str1);
	len2 = ft_strlen(str2);
	result = (char *)gc_malloc(gc, len1 + len2 + 1);
	if (!result)
		return (NULL);
	ft_strlcpy(result, str1, len1 + 1);
	ft_strlcat(result, str2, len1 + len2 + 1);
	return (result);
}

char	*ft_strdup_gc(t_gc *gc, const char *s)
{
	char	*dup;
	size_t	len;

	if (!s)
		return (NULL);
	len = ft_strlen(s);
	dup = (char *)gc_malloc(gc, len + 1);
	if (!dup)
		return (NULL);
	ft_strlcpy(dup, s, len + 1);
	return (dup);
}

int	is_whitespace(char c)
{
	return (c == ' ' || c == '\t');
}

void	skip_whitespace(const char **input)
{
	while (**input && is_whitespace(**input))
		(*input)++;
}
