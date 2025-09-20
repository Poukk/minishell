/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_redir_helpers.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexanfe <alexanfe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 01:22:42 by alexanfe          #+#    #+#             */
/*   Updated: 2025/09/19 16:47:34 by alexanfe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	process_input_redirection(t_gc *gc, t_token **tokens,
		t_redirection **input_redirs)
{
	t_redirection	*new_redir;

	*tokens = (*tokens)->next;
	if (*tokens && (*tokens)->type == TOKEN_WORD)
	{
		new_redir = redirection_create(gc, TOKEN_REDIR_IN, (*tokens)->value);
		if (new_redir)
			redirection_add_back(input_redirs, new_redir);
		*tokens = (*tokens)->next;
	}
}

void	process_output_redirection(t_gc *gc, t_token **tokens,
		t_redirection **output_redirs, t_token_type type)
{
	t_redirection	*new_redir;

	*tokens = (*tokens)->next;
	if (*tokens && (*tokens)->type == TOKEN_WORD)
	{
		new_redir = redirection_create(gc, type, (*tokens)->value);
		if (new_redir)
			redirection_add_back(output_redirs, new_redir);
		*tokens = (*tokens)->next;
	}
}

int	is_redirection_token(t_token_type type)
{
	return (type == TOKEN_REDIR_IN
		|| type == TOKEN_REDIR_OUT
		|| type == TOKEN_REDIR_APPEND
		|| type == TOKEN_HEREDOC);
}
