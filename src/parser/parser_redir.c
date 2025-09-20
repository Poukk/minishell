/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_redir.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexanfe <alexanfe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 01:22:42 by alexanfe          #+#    #+#             */
/*   Updated: 2025/09/19 16:47:06 by alexanfe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	parse_redirections(t_gc *gc, t_token **tokens,
		t_redirection_entry **redirections, int *redir_position)
{
	t_redirection_entry	*entry;

	while (*tokens && is_redirection_token((*tokens)->type))
	{
		entry = process_redirection_entry(gc, tokens, *redir_position);
		if (entry)
		{
			redirection_entry_add_ordered(redirections, entry);
			(*redir_position)++;
		}
	}
}

void	parse_redirections_legacy(t_gc *gc, t_token **tokens,
		t_redirection **input_redirs, t_redirection **output_redirs)
{
	while (*tokens && is_redirection_token((*tokens)->type))
	{
		if ((*tokens)->type == TOKEN_REDIR_IN)
			process_input_redirection(gc, tokens, input_redirs);
		else if ((*tokens)->type == TOKEN_HEREDOC)
			process_heredoc_redirection(gc, tokens, input_redirs);
		else if ((*tokens)->type == TOKEN_REDIR_OUT)
			process_output_redirection(gc, tokens, output_redirs,
				TOKEN_REDIR_OUT);
		else if ((*tokens)->type == TOKEN_REDIR_APPEND)
			process_output_redirection(gc, tokens, output_redirs,
				TOKEN_REDIR_APPEND);
	}
}

t_redirection_entry	*process_redirection_entry(t_gc *gc, t_token **tokens, int position)
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

t_redirection_entry	*process_heredoc_entry(t_gc *gc, t_token **tokens, int position)
{
	t_redirection_entry	*entry;
	char				*content;

	content = collect_heredoc_content(gc, (*tokens)->value);
	if (content)
		content = expand_heredoc_variables(gc, content, (*tokens)->value);
	entry = heredoc_entry_create(gc, (*tokens)->value, content, position);
	return (entry);
}

static int	is_word_or_redirection_token(t_token_type type)
{
	return (type == TOKEN_WORD
		|| type == TOKEN_VARIABLE
		|| type == TOKEN_REDIR_IN
		|| type == TOKEN_REDIR_OUT
		|| type == TOKEN_REDIR_APPEND
		|| type == TOKEN_HEREDOC);
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
		t_redir_params *params)
{
	t_parse_context	ctx;
	char			**args;
	int				i;
	int				count;

	count = count_command_tokens(*tokens);
	args = init_args_array(gc, *tokens);
	if (!args)
		return (NULL);
	i = 0;
	init_parse_context(&ctx, args, &i, count);
	ctx.redirections = params->redirections;
	ctx.redir_position = *(params->redir_position);
	ctx.input_redirs = params->input_redirs;
	ctx.output_redirs = params->output_redirs;
	while (*tokens && is_word_or_redirection_token((*tokens)->type))
		process_token(gc, tokens, &ctx, params->env);
	args[i] = NULL;
	return (args);
}
