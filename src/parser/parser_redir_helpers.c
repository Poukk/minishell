/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_redir_helpers.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexanfe <alexanfe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 01:22:42 by alexanfe          #+#    #+#             */
/*   Updated: 2025/09/15 01:22:43 by alexanfe         ###   ########.fr       */
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
	return (type == TOKEN_REDIR_IN || type == TOKEN_REDIR_OUT
		|| type == TOKEN_REDIR_APPEND);
}

void	process_word_token(t_gc *gc, t_token **tokens, t_parse_context *ctx)
{
	ctx->args[*(ctx->i)] = gc_malloc(gc, ft_strlen((*tokens)->value) + 1);
	if (ctx->args[*(ctx->i)])
	{
		ft_strlcpy(ctx->args[*(ctx->i)], (*tokens)->value,
			ft_strlen((*tokens)->value) + 1);
		*tokens = (*tokens)->next;
		(*(ctx->i))++;
	}
}

void	process_token(t_gc *gc, t_token **tokens, t_parse_context *ctx)
{
	if ((*tokens)->type == TOKEN_WORD && *(ctx->i) < ctx->max_args)
		process_word_token(gc, tokens, ctx);
	else
		parse_redirections(gc, tokens, ctx->input_redirs, ctx->output_redirs);
}
