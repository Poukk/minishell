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

static t_token	*handle_variable(t_gc *gc, const char **input)
{
	const char	*start;
	size_t		len;
	char		*var_name;

	(*input)++;
	start = *input;
	if (**input == '?')
	{
		(*input)++;
		return (token_create(gc, TOKEN_VARIABLE, "?"));
	}
	while (**input && (ft_isalnum(**input) || **input == '_'))
		(*input)++;
	len = *input - start;
	if (len == 0)
		return (token_create(gc, TOKEN_WORD, "$"));
	var_name = (char *)gc_malloc(gc, len + 1);
	if (!var_name)
		return (NULL);
	ft_strlcpy(var_name, start, len + 1);
	return (token_create(gc, TOKEN_VARIABLE, var_name));
}

t_token	*handle_metachar(t_gc *gc, const char **input)
{
	if (**input == '|')
		return (handle_pipe(gc, input));
	else if (**input == '<')
		return (handle_redirect_in(gc, input));
	else if (**input == '>')
		return (handle_redirect_out(gc, input));
	else if (**input == '$')
		return (handle_variable(gc, input));
	return (NULL);
}
