/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   content.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexanfe <alexanfe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/19 17:49:15 by alexanfe          #+#    #+#             */
/*   Updated: 2025/09/19 17:49:15 by alexanfe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*extract_word(t_gc *gc, const char **input)
{
	const char	*start;
	size_t		len;
	char		*result;

	start = *input;
	while (**input && !is_whitespace(**input) && !is_metachar(**input)
		&& **input != '\'' && **input != '"')
		(*input)++;
	len = *input - start;
	if (len == 0)
		return (NULL);
	result = (char *)gc_malloc(gc, len + 1);
	if (!result)
		return (NULL);
	ft_strlcpy(result, start, len + 1);
	return (result);
}

static void	update_quote_context(char current_quote, char *quote_context)
{
	if (current_quote == '\'' && *quote_context != '\'')
		*quote_context = '\'';
	else if (current_quote == '"' && *quote_context == 0)
		*quote_context = '"';
}

char	*extract_variable_content(t_gc *gc, const char **input)
{
	const char	*start;
	size_t		len;

	(*input)++;
	if (**input == '?')
		return (handle_special_var(gc, input));
	start = *input;
	while (**input && (ft_isalnum(**input) || **input == '_'))
		(*input)++;
	len = *input - start;
	if (len == 0)
		return (handle_empty_var(gc));
	return (handle_normal_var(gc, start, len));
}

char	*get_content_by_type(t_gc *gc, const char **input,
		char *current_quote)
{
	char	*temp_content;

	*current_quote = 0;
	if (**input == '\'')
	{
		temp_content = extract_quoted_string(gc, input, '\'');
		*current_quote = '\'';
	}
	else if (**input == '"')
	{
		temp_content = extract_quoted_string(gc, input, '"');
		*current_quote = '"';
	}
	else if (**input == '$')
		temp_content = extract_variable_content(gc, input);
	else
		temp_content = extract_word(gc, input);
	return (temp_content);
}

t_concat_result	extract_concatenated_content_with_context(t_gc *gc,
		const char **input)
{
	t_concat_result	result;
	char			*temp_content;
	char			current_quote;

	result.content = NULL;
	result.quote_context = 0;
	while (**input && should_concatenate(*input))
	{
		temp_content = get_content_by_type(gc, input, &current_quote);
		if (!temp_content)
			return (result);
		result.content = concatenate_strings(gc, result.content, temp_content);
		if (!result.content)
		{
			result.quote_context = 0;
			return (result);
		}
		update_quote_context(current_quote, &result.quote_context);
	}
	return (result);
}
