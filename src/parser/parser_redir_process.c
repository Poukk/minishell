/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_redir_process.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexanfe <alexanfe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 01:22:42 by alexanfe          #+#    #+#             */
/*   Updated: 2025/09/15 01:22:43 by alexanfe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	process_word_token(t_gc *gc, t_token **tokens, t_parse_context *ctx,
		t_shell_env *env)
{
	char	*var_with_dollar;
	size_t	len;

	(void)env;
	if ((*tokens)->type == TOKEN_VARIABLE)
	{
		len = ft_strlen((*tokens)->value) + 2;
		var_with_dollar = gc_malloc(gc, len);
		if (!var_with_dollar)
			return ;
		var_with_dollar[0] = '$';
		ft_strlcpy(var_with_dollar + 1, (*tokens)->value, len - 1);
		ctx->args[*(ctx->i)] = var_with_dollar;
	}
	else
	{
		ctx->args[*(ctx->i)] = gc_malloc(gc, ft_strlen((*tokens)->value) + 1);
		if (!ctx->args[*(ctx->i)])
			return ;
		ft_strlcpy(ctx->args[*(ctx->i)], (*tokens)->value,
			ft_strlen((*tokens)->value) + 1);
	}
	*tokens = (*tokens)->next;
	(*(ctx->i))++;
}

void	process_token(t_gc *gc, t_token **tokens, t_parse_context *ctx,
		t_shell_env *env)
{
	if (((*tokens)->type == TOKEN_WORD || (*tokens)->type == TOKEN_VARIABLE)
		&& *(ctx->i) < ctx->max_args)
		process_word_token(gc, tokens, ctx, env);
	else
		parse_redirections(gc, tokens, ctx->input_redirs, ctx->output_redirs);
}
