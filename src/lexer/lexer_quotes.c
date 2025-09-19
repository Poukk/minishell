/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_quotes.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexanfe <alexanfe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 17:26:33 by alexanfe          #+#    #+#             */
/*   Updated: 2025/08/22 17:26:34 by alexanfe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
