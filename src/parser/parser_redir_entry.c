/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_redir_entry.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexanfe <alexanfe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 12:00:00 by alexanfe          #+#    #+#             */
/*   Updated: 2025/01/20 12:00:00 by alexanfe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_redirection_entry	*process_redirection_entry(t_gc *gc, t_token **tokens,
	int position)
{
	t_redirection_entry	*entry;
	t_token_type		type;

	type = (*tokens)->type;
	*tokens = (*tokens)->next;
	if (!*tokens || (*tokens)->type != TOKEN_WORD)
	{
		ft_dprintf(2, "minishell: syntax error near unexpected token\n");
		return (NULL);
	}
	if (type == TOKEN_HEREDOC)
		entry = process_heredoc_entry(gc, tokens, position);
	else
		entry = redirection_entry_create(gc, type, (*tokens)->value, position);
	if (entry)
		*tokens = (*tokens)->next;
	return (entry);
}

t_redirection_entry	*process_heredoc_entry(t_gc *gc, t_token **tokens,
	int position)
{
	t_redirection_entry	*entry;
	char				*content;

	content = collect_heredoc_content(gc, (*tokens)->value);
	if (content)
		content = expand_heredoc_variables(gc, content, (*tokens)->value);
	entry = heredoc_entry_create(gc, (*tokens)->value, content, position);
	return (entry);
}
