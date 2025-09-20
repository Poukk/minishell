/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_redir_count.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexanfe <alexanfe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 12:00:00 by alexanfe          #+#    #+#             */
/*   Updated: 2025/01/20 12:00:00 by alexanfe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	skip_initial_redirections(t_token **current)
{
	while (*current && ((*current)->type == TOKEN_REDIR_IN
			|| (*current)->type == TOKEN_REDIR_OUT
			|| (*current)->type == TOKEN_REDIR_APPEND
			|| (*current)->type == TOKEN_HEREDOC))
	{
		*current = (*current)->next;
		if (*current && (*current)->type == TOKEN_WORD)
			*current = (*current)->next;
	}
}

static int	count_args_with_redirs(t_token **current)
{
	int	count;

	count = 0;
	while (*current && ((*current)->type == TOKEN_WORD
			|| (*current)->type == TOKEN_VARIABLE))
	{
		count++;
		*current = (*current)->next;
		skip_redirections(current);
	}
	return (count);
}

int	count_command_tokens(t_token *tokens)
{
	t_token	*current;

	current = tokens;
	skip_initial_redirections(&current);
	return (count_args_with_redirs(&current));
}

void	skip_redirections(t_token **current)
{
	while (*current && ((*current)->type == TOKEN_REDIR_IN
			|| (*current)->type == TOKEN_REDIR_OUT
			|| (*current)->type == TOKEN_REDIR_APPEND
			|| (*current)->type == TOKEN_HEREDOC))
	{
		*current = (*current)->next;
		if (*current && (*current)->type == TOKEN_WORD)
			*current = (*current)->next;
	}
}
