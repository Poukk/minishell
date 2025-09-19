/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexanfe <alexanfe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 17:26:51 by alexanfe          #+#    #+#             */
/*   Updated: 2025/09/19 16:41:17 by alexanfe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*token_create_with_quote(t_gc *gc, t_token_type type,
					const char *value, char quote_context)
{
	t_token	*token;

	token = (t_token *)gc_malloc(gc, sizeof(t_token));
	if (!token)
		return (NULL);
	token->type = type;
	token->quote_context = quote_context;
	if (value)
	{
		token->value = (char *)gc_malloc(gc, ft_strlen(value) + 1);
		if (!token->value)
			return (NULL);
		ft_strlcpy(token->value, value, ft_strlen(value) + 1);
	}
	else
		token->value = NULL;
	token->next = NULL;
	return (token);
}

char	*handle_special_var(t_gc *gc, const char **input)
{
	char	*result;

	(*input)++;
	result = (char *)gc_malloc(gc, 3);
	if (!result)
		return (NULL);
	ft_strlcpy(result, "$?", 3);
	return (result);
}

char	*handle_empty_var(t_gc *gc)
{
	char	*result;

	result = (char *)gc_malloc(gc, 2);
	if (!result)
		return (NULL);
	ft_strlcpy(result, "$", 2);
	return (result);
}

char	*handle_normal_var(t_gc *gc, const char *start, size_t len)
{
	char	*result;

	result = (char *)gc_malloc(gc, len + 2);
	if (!result)
		return (NULL);
	result[0] = '$';
	ft_strlcpy(result + 1, start, len + 1);
	return (result);
}

t_token	*handle_word_or_quote(t_gc *gc, const char **input)
{
	char	*value;

	if (**input == '\'')
	{
		value = extract_quoted_string(gc, input, '\'');
		if (!value)
			return (NULL);
		return (token_create_with_quote(gc, TOKEN_WORD, value, '\''));
	}
	else if (**input == '"')
	{
		value = extract_quoted_string(gc, input, '"');
		if (!value)
			return (NULL);
		return (token_create_with_quote(gc, TOKEN_WORD, value, '"'));
	}
	else
	{
		value = extract_word(gc, input);
		if (!value)
			return (NULL);
		return (token_create(gc, TOKEN_WORD, value));
	}
}
