/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexanfe <alexanfe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/19 16:32:00 by alexanfe          #+#    #+#             */
/*   Updated: 2025/09/19 17:04:25 by alexanfe         ###   ########.fr       */
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
		return (gc_strdup(gc, str2));
	if (!str2)
		return (gc_strdup(gc, str1));
	len1 = ft_strlen(str1);
	len2 = ft_strlen(str2);
	result = (char *)gc_malloc(gc, len1 + len2 + 1);
	if (!result)
		return (NULL);
	ft_strlcpy(result, str1, len1 + 1);
	ft_strlcat(result, str2, len1 + len2 + 1);
	return (result);
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
