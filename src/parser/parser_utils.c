/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexanfe <alexanfe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 01:22:42 by alexanfe          #+#    #+#             */
/*   Updated: 2025/09/02 01:22:43 by alexanfe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	count_command_tokens(t_token *tokens)
{
	int		count;
	t_token	*current;

	count = 0;
	current = tokens;
	while (current && current->type == TOKEN_WORD)
	{
		count++;
		current = current->next;
		/* TODO: PHASE 5 - Remove this temporary redirection skipping logic.
		 * This is a Phase 2 workaround to handle pipes mixed with redirections.
		 * In Phase 5, redirections should be properly parsed and stored in AST.
		 */
		while (current && (current->type == TOKEN_REDIR_IN
				|| current->type == TOKEN_REDIR_OUT
				|| current->type == TOKEN_REDIR_APPEND
				|| current->type == TOKEN_HEREDOC))
		{
			current = current->next;
			if (current && current->type == TOKEN_WORD)
				current = current->next;
		}
	}
	return (count);
}

char	**extract_command_args(t_gc *gc, t_token **tokens)
{
	char	**args;
	int		count;
	int		i;

	count = count_command_tokens(*tokens);
	if (count == 0)
		return (NULL);
	args = gc_malloc(gc, sizeof(char *) * (count + 1));
	if (!args)
		return (NULL);
	i = 0;
	while (i < count && *tokens && (*tokens)->type == TOKEN_WORD)
	{
		args[i] = gc_malloc(gc, ft_strlen((*tokens)->value) + 1);
		if (!args[i])
			return (NULL);
		ft_strlcpy(args[i], (*tokens)->value, ft_strlen((*tokens)->value) + 1);
		*tokens = (*tokens)->next;
		i++;
		/* TODO: PHASE 5 - Remove this temporary redirection skipping logic.
		 * This is a Phase 2 workaround to handle pipes mixed with redirections.
		 * In Phase 5, redirections should be properly parsed and stored in AST.
		 */
		while (*tokens && ((*tokens)->type == TOKEN_REDIR_IN
				|| (*tokens)->type == TOKEN_REDIR_OUT
				|| (*tokens)->type == TOKEN_REDIR_APPEND
				|| (*tokens)->type == TOKEN_HEREDOC))
		{
			*tokens = (*tokens)->next;
			if (*tokens && (*tokens)->type == TOKEN_WORD)
				*tokens = (*tokens)->next;
		}
	}
	args[i] = NULL;
	return (args);
}
