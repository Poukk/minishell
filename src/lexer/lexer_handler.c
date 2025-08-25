/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_handler.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexanfe <alexanfe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 17:26:33 by alexanfe          #+#    #+#             */
/*   Updated: 2025/08/22 17:26:34 by alexanfe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_token	*handle_pipe(t_gc *gc, const char **input)
{
	(*input)++;
	return (token_create(gc, TOKEN_PIPE, "|"));
}

static t_token	*handle_redirect_in(t_gc *gc, const char **input)
{
	if (*(*input + 1) == '<')
	{
		*input += 2;
		return (token_create(gc, TOKEN_HEREDOC, "<<"));
	}
	(*input)++;
	return (token_create(gc, TOKEN_REDIR_IN, "<"));
}

static t_token	*handle_redirect_out(t_gc *gc, const char **input)
{
	if (*(*input + 1) == '>')
	{
		*input += 2;
		return (token_create(gc, TOKEN_REDIR_APPEND, ">>"));
	}
	(*input)++;
	return (token_create(gc, TOKEN_REDIR_OUT, ">"));
}

t_token	*handle_metachar(t_gc *gc, const char **input)
{
	if (**input == '|')
		return (handle_pipe(gc, input));
	else if (**input == '<')
		return (handle_redirect_in(gc, input));
	else if (**input == '>')
		return (handle_redirect_out(gc, input));
	return (NULL);
}

t_token	*handle_word_or_quote(t_gc *gc, const char **input)
{
	char	*value;

	if (**input == '\'')
	{
		value = extract_quoted_string(gc, input, '\'');
		if (!value)
			return (NULL);
		return (token_create(gc, TOKEN_WORD, value));
	}
	else if (**input == '"')
	{
		value = extract_quoted_string(gc, input, '"');
		if (!value)
			return (NULL);
		return (token_create(gc, TOKEN_WORD, value));
	}
	else
	{
		value = extract_word(gc, input);
		if (!value)
			return (NULL);
		return (token_create(gc, TOKEN_WORD, value));
	}
}
