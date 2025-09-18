/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexanfe <alexanfe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 17:26:42 by alexanfe          #+#    #+#             */
/*   Updated: 2025/08/22 17:26:43 by alexanfe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_whitespace(char c)
{
	return (c == ' ' || c == '\t');
}

static int	is_metachar(char c)
{
	return (c == '|' || c == '<' || c == '>' || c == '$');
}

static void	skip_whitespace(const char **input)
{
	while (**input && is_whitespace(**input))
		(*input)++;
}

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

t_token	*lexer_tokenize(t_gc *gc, const char *input)
{
	t_token		*head;
	t_token		*token;
	const char	*current;

	head = NULL;
	current = input;
	while (*current)
	{
		skip_whitespace(&current);
		if (!*current)
			break ;
		if (is_metachar(*current))
			token = handle_metachar(gc, &current);
		else
			token = handle_word_or_quote(gc, &current);
		if (!token)
			return (NULL);
		token_add_back(&head, token);
	}
	token = token_create(gc, TOKEN_EOF, NULL);
	if (token)
		token_add_back(&head, token);
	return (head);
}
