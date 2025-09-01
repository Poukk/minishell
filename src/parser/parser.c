/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexanfe <alexanfe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/01 00:00:00 by alexanfe          #+#    #+#             */
/*   Updated: 2025/09/01 00:00:00 by alexanfe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_ast_node	*parse_pipe(t_gc *gc, t_token **tokens);
static t_ast_node	*parse_command(t_gc *gc, t_token **tokens);

t_ast_node	*parser_parse(t_gc *gc, t_token *tokens)
{
	t_token	*current;

	if (!tokens)
		return (NULL);
	if (tokens->type == TOKEN_PIPE)
		return (NULL);
	current = tokens;
	return (parse_pipe(gc, &current));
}

static t_ast_node	*parse_pipe(t_gc *gc, t_token **tokens)
{
	t_ast_node	*left;
	t_ast_node	*pipe_node;

	left = parse_command(gc, tokens);
	if (!left)
		return (NULL);
	if (*tokens && (*tokens)->type == TOKEN_PIPE)
	{
		*tokens = (*tokens)->next;
		if (!*tokens || (*tokens)->type == TOKEN_PIPE)
			return (NULL);
		pipe_node = ast_node_create(gc, NODE_PIPE);
		if (!pipe_node)
			return (NULL);
		pipe_node->left = left;
		pipe_node->right = parse_pipe(gc, tokens);
		if (!pipe_node->right)
			return (NULL);
		return (pipe_node);
	}
	return (left);
}

static t_ast_node	*parse_command(t_gc *gc, t_token **tokens)
{
	t_ast_node	*cmd_node;
	char		**args;

	if (!*tokens || (*tokens)->type != TOKEN_WORD)
		return (NULL);
	cmd_node = ast_node_create(gc, NODE_CMD);
	if (!cmd_node)
		return (NULL);
	args = extract_command_args(gc, tokens);
	cmd_node->args = args;
	return (cmd_node);
}
