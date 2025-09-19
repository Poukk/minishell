/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexanfe <alexanfe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/19 17:49:28 by alexanfe          #+#    #+#             */
/*   Updated: 2025/09/19 17:49:28 by alexanfe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_metachar(char c)
{
	return (c == '|' || c == '<' || c == '>' || c == '$');
}

int	is_separator_metachar(char c)
{
	return (c == '|' || c == '<' || c == '>');
}

int	should_concatenate(const char *current)
{
	return (*current && !is_whitespace(*current)
		&& !is_separator_metachar(*current));
}

char	*extract_quoted_string(t_gc *gc, const char **input, char quote)
{
	const char	*start;
	size_t		len;
	char		*result;

	(*input)++;
	start = *input;
	while (**input && **input != quote)
		(*input)++;
	if (**input == quote)
	{
		len = *input - start;
		result = (char *)gc_malloc(gc, len + 1);
		if (!result)
			return (NULL);
		ft_strlcpy(result, start, len + 1);
		(*input)++;
		return (result);
	}
	return (NULL);
}
