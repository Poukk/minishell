/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_redir.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexanfe <alexanfe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 01:22:42 by alexanfe          #+#    #+#             */
/*   Updated: 2025/09/15 01:22:43 by alexanfe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	parse_redirections(t_gc *gc, t_token **tokens,
		t_redirection **input_redirs, t_redirection **output_redirs)
{
	while (*tokens && is_redirection_token((*tokens)->type))
	{
		if ((*tokens)->type == TOKEN_REDIR_IN)
			process_input_redirection(gc, tokens, input_redirs);
		else if ((*tokens)->type == TOKEN_REDIR_OUT)
			process_output_redirection(gc, tokens, output_redirs,
				TOKEN_REDIR_OUT);
		else if ((*tokens)->type == TOKEN_REDIR_APPEND)
			process_output_redirection(gc, tokens, output_redirs,
				TOKEN_REDIR_APPEND);
	}
}

static int	is_word_or_redirection_token(t_token_type type)
{
	return (type == TOKEN_WORD || type == TOKEN_REDIR_IN
		|| type == TOKEN_REDIR_OUT || type == TOKEN_REDIR_APPEND);
}

static char	**init_args_array(t_gc *gc, t_token *tokens)
{
	char	**args;
	int		count;

	count = count_command_tokens(tokens);
	if (count == 0)
		return (NULL);
	args = gc_malloc(gc, sizeof(char *) * (count + 1));
	return (args);
}

static void	init_parse_context(t_parse_context *ctx, char **args,
		int *i, int max_args)
{
	ctx->args = args;
	ctx->i = i;
	ctx->max_args = max_args;
}

char	**extract_args_with_redirections(t_gc *gc, t_token **tokens,
		t_redirection **input_redirs, t_redirection **output_redirs)
{
	char			**args;
	int				i;
	int				count;
	t_parse_context	ctx;

	count = count_command_tokens(*tokens);
	args = init_args_array(gc, *tokens);
	if (!args)
		return (NULL);
	i = 0;
	init_parse_context(&ctx, args, &i, count);
	ctx.input_redirs = input_redirs;
	ctx.output_redirs = output_redirs;
	while (*tokens && is_word_or_redirection_token((*tokens)->type))
		process_token(gc, tokens, &ctx);
	args[i] = NULL;
	return (args);
}
