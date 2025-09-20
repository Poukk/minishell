/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_token_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexanfe <alexanfe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 12:00:00 by alexanfe          #+#    #+#             */
/*   Updated: 2025/01/20 12:00:00 by alexanfe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_word_or_redirection_token(t_token_type type)
{
	return (type == TOKEN_WORD
		|| type == TOKEN_VARIABLE
		|| type == TOKEN_REDIR_IN
		|| type == TOKEN_REDIR_OUT
		|| type == TOKEN_REDIR_APPEND
		|| type == TOKEN_HEREDOC);
}

int	has_command_token(t_token *tokens)
{
	t_token	*current;

	current = tokens;
	while (current && (current->type == TOKEN_REDIR_IN
			|| current->type == TOKEN_REDIR_OUT
			|| current->type == TOKEN_REDIR_APPEND
			|| current->type == TOKEN_HEREDOC))
	{
		current = current->next;
		if (current && current->type == TOKEN_WORD)
			current = current->next;
	}
	return (current && (current->type == TOKEN_WORD
			|| current->type == TOKEN_VARIABLE));
}

void	init_redir_ptrs(t_redirection **input, t_redirection **output,
	t_redirection_entry **redirections, int *redir_position)
{
	*input = NULL;
	*output = NULL;
	*redirections = NULL;
	*redir_position = 0;
}
