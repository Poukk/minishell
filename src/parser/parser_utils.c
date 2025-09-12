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
		while (current && (current->type == TOKEN_REDIR_IN
				|| current->type == TOKEN_REDIR_OUT))
		{
			current = current->next;
			if (current && current->type == TOKEN_WORD)
				current = current->next;
		}
	}
	return (count);
}

static void	skip_redirections(t_token **current)
{
	while (*current && ((*current)->type == TOKEN_REDIR_IN
			|| (*current)->type == TOKEN_REDIR_OUT))
	{
		*current = (*current)->next;
		if (*current && (*current)->type == TOKEN_WORD)
			*current = (*current)->next;
	}
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
		skip_redirections(tokens);
	}
	args[i] = NULL;
	return (args);
}

void	parse_redirections(t_gc *gc, t_token **tokens,
		t_redirection **input_redirs, t_redirection **output_redirs)
{
	t_redirection	*new_redir;

	while (*tokens && ((*tokens)->type == TOKEN_REDIR_IN
			|| (*tokens)->type == TOKEN_REDIR_OUT))
	{
		if ((*tokens)->type == TOKEN_REDIR_IN)
		{
			*tokens = (*tokens)->next;
			if (*tokens && (*tokens)->type == TOKEN_WORD)
			{
				new_redir = redirection_create(gc, TOKEN_REDIR_IN,
						(*tokens)->value);
				if (new_redir)
					redirection_add_back(input_redirs, new_redir);
				*tokens = (*tokens)->next;
			}
		}
		else if ((*tokens)->type == TOKEN_REDIR_OUT)
		{
			*tokens = (*tokens)->next;
			if (*tokens && (*tokens)->type == TOKEN_WORD)
			{
				new_redir = redirection_create(gc, TOKEN_REDIR_OUT,
						(*tokens)->value);
				if (new_redir)
					redirection_add_back(output_redirs, new_redir);
				*tokens = (*tokens)->next;
			}
		}
	}
}

static void	process_word_token(t_gc *gc, t_token **tokens, t_parse_context *ctx)
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

static void	process_token(t_gc *gc, t_token **tokens, t_parse_context *ctx)
{
	if ((*tokens)->type == TOKEN_WORD && *(ctx->i) < ctx->max_args)
		process_word_token(gc, tokens, ctx);
	else
		parse_redirections(gc, tokens, ctx->input_redirs, ctx->output_redirs);
}

char	**extract_args_with_redirections(t_gc *gc, t_token **tokens,
		t_redirection **input_redirs, t_redirection **output_redirs)
{
	char			**args;
	int				count;
	int				i;
	t_parse_context	ctx;

	count = count_command_tokens(*tokens);
	if (count == 0)
		return (NULL);
	args = gc_malloc(gc, sizeof(char *) * (count + 1));
	if (!args)
		return (NULL);
	i = 0;
	ctx.args = args;
	ctx.i = &i;
	ctx.max_args = count;
	ctx.input_redirs = input_redirs;
	ctx.output_redirs = output_redirs;
	while (*tokens && ((*tokens)->type == TOKEN_WORD
			|| (*tokens)->type == TOKEN_REDIR_IN
			|| (*tokens)->type == TOKEN_REDIR_OUT))
	{
		process_token(gc, tokens, &ctx);
	}
	args[i] = NULL;
	return (args);
}
