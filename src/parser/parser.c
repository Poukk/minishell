/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexanfe <alexanfe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 01:22:36 by alexanfe          #+#    #+#             */
/*   Updated: 2025/09/19 16:41:50 by alexanfe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_ast_node	*parse(t_gc *gc, t_token *tokens, t_shell_env *env)
{
	t_token	*current;

	if (!tokens)
		return (NULL);
	if (tokens->type == TOKEN_PIPE)
		return (NULL);
	current = tokens;
	return (parse_pipe(gc, &current, env));
}

t_ast_node	*parse_pipe(t_gc *gc, t_token **tokens, t_shell_env *env)
{
	t_ast_node	*left;
	t_ast_node	*pipe_node;

	left = parse_command(gc, tokens, env);
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
		pipe_node->right = parse_pipe(gc, tokens, env);
		if (!pipe_node->right)
			return (NULL);
		return (pipe_node);
	}
	return (left);
}

static void	set_redir_params(t_redir_params *params, t_redir_ptrs *ptrs)
{
	params->redirections = &ptrs->redirections;
	params->redir_position = &ptrs->redir_position;
	params->input_redirs = &ptrs->input_redirs;
	params->output_redirs = &ptrs->output_redirs;
}

static void	set_cmd_redirections(t_ast_node *cmd_node,
	t_redirection *input_redirs, t_redirection *output_redirs,
	t_redirection_entry *redirections)
{
	cmd_node->redirections = redirections;
	cmd_node->input_redirs = input_redirs;
	cmd_node->output_redirs = output_redirs;
}

t_ast_node	*parse_command(t_gc *gc, t_token **tokens, t_shell_env *env)
{
	t_ast_node		*cmd_node;
	t_redir_params	params;
	t_redir_ptrs	ptrs;

	if (!*tokens || (!has_command_token(*tokens)
			&& !is_redirection_token((*tokens)->type)))
		return (NULL);
	cmd_node = ast_node_create(gc, NODE_CMD);
	if (!cmd_node)
		return (NULL);
	ptrs.input_redirs = NULL;
	ptrs.output_redirs = NULL;
	ptrs.redirections = NULL;
	ptrs.redir_position = 0;
	set_redir_params(&params, &ptrs);
	params.env = env;
	cmd_node->args = extract_args_with_redirections(gc, tokens, &params);
	set_cmd_redirections(cmd_node, ptrs.input_redirs, ptrs.output_redirs,
		ptrs.redirections);
	return (cmd_node);
}
