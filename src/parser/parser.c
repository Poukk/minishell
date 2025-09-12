/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexanfe <alexanfe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 01:22:36 by alexanfe          #+#    #+#             */
/*   Updated: 2025/09/02 01:22:37 by alexanfe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

t_ast_node	*parse_pipe(t_gc *gc, t_token **tokens)
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

t_ast_node	*parse_command(t_gc *gc, t_token **tokens)
{
	t_ast_node		*cmd_node;
	char			**args;
	t_redirection	*input_redirs;
	t_redirection	*output_redirs;

	if (!*tokens || (*tokens)->type != TOKEN_WORD)
		return (NULL);
	cmd_node = ast_node_create(gc, NODE_CMD);
	if (!cmd_node)
		return (NULL);
	input_redirs = NULL;
	output_redirs = NULL;
	args = extract_args_with_redirections(gc, tokens,
			&input_redirs, &output_redirs);
	cmd_node->args = args;
	cmd_node->input_redirs = input_redirs;
	cmd_node->output_redirs = output_redirs;
	return (cmd_node);
}
