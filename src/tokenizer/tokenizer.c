/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexanfe <alexanfe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/19 17:49:35 by alexanfe          #+#    #+#             */
/*   Updated: 2025/09/19 17:49:36 by alexanfe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_token	*create_word_token(t_gc *gc, const char **current)
{
	t_concat_result	concat_result;

	concat_result = extract_concatenated_content_with_context(gc, current);
	if (!concat_result.content)
		return (NULL);
	return (token_create_with_quote(gc, TOKEN_WORD,
			concat_result.content, concat_result.quote_context));
}

static t_token	*process_input_token(t_gc *gc, const char **current)
{
	if (is_separator_metachar(**current))
		return (handle_metachar(gc, current));
	else
		return (create_word_token(gc, current));
}

static void	token_add_back(t_token **head, t_token *new_token)
{
	t_token	*current;

	if (!head || !new_token)
		return ;
	if (!*head)
	{
		*head = new_token;
		return ;
	}
	current = *head;
	while (current->next)
		current = current->next;
	current->next = new_token;
}

t_token	*token_create(t_gc *gc, t_token_type type, const char *value)
{
	return (token_create_with_quote(gc, type, value, 0));
}

t_token	*tokenize(t_gc *gc, const char *input)
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
		token = process_input_token(gc, &current);
		if (!token)
			return (NULL);
		token_add_back(&head, token);
	}
	token = token_create(gc, TOKEN_EOF, NULL);
	if (token)
		token_add_back(&head, token);
	return (head);
}
