/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexanfe <alexanfe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 17:26:51 by alexanfe          #+#    #+#             */
/*   Updated: 2025/08/22 17:26:52 by alexanfe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*token_create(t_gc *gc, t_token_type type, const char *value)
{
	t_token	*token;

	token = (t_token *)gc_malloc(gc, sizeof(t_token));
	if (!token)
		return (NULL);
	token->type = type;
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

void	token_add_back(t_token **head, t_token *new_token)
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

void	token_print_list(t_token *tokens)
{
	const char	*type_names[] = {
		"WORD", "PIPE", "REDIR_IN", "REDIR_OUT",
		"REDIR_APPEND", "HEREDOC", "VARIABLE", "EOF"
	};

	while (tokens)
	{
		ft_printf("Token: %s", type_names[tokens->type]);
		if (tokens->value)
			ft_printf(" | Value: '%s'", tokens->value);
		ft_printf("\n");
		tokens = tokens->next;
	}
}
