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

static char	*create_prefixed_arg(t_gc *gc, const char *prefix,
		const char *value)
{
	char	*prefixed_arg;
	size_t	len;

	len = ft_strlen(value) + ft_strlen(prefix) + 1;
	prefixed_arg = gc_malloc(gc, len);
	if (!prefixed_arg)
		return (NULL);
	ft_strlcpy(prefixed_arg, prefix, len);
	ft_strlcat(prefixed_arg, value, len);
	return (prefixed_arg);
}

static void	process_variable_token(t_gc *gc, t_token *token, char **arg_slot)
{
	char	*var_with_dollar;
	size_t	len;

	len = ft_strlen(token->value) + 2;
	var_with_dollar = gc_malloc(gc, len);
	if (!var_with_dollar)
		return ;
	var_with_dollar[0] = '$';
	ft_strlcpy(var_with_dollar + 1, token->value, len - 1);
	*arg_slot = var_with_dollar;
}

static void	process_quoted_token(t_gc *gc, t_token *token, char **arg_slot)
{
	if (token->quote_context == '\'')
		*arg_slot = create_prefixed_arg(gc, "SINGLE_QUOTE:", token->value);
	else if (token->quote_context == '"')
		*arg_slot = create_prefixed_arg(gc, "DOUBLE_QUOTE:", token->value);
	else
	{
		*arg_slot = gc_malloc(gc, ft_strlen(token->value) + 1);
		if (*arg_slot)
			ft_strlcpy(*arg_slot, token->value, ft_strlen(token->value) + 1);
	}
}

void	process_word_token(t_gc *gc, t_token **tokens, t_parse_context *ctx,
		t_shell_env *env)
{
	(void)env;
	if ((*tokens)->type == TOKEN_VARIABLE)
		process_variable_token(gc, *tokens, &ctx->args[*(ctx->i)]);
	else
		process_quoted_token(gc, *tokens, &ctx->args[*(ctx->i)]);
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
