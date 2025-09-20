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

static int	has_command_token(t_token *tokens)
{
	t_token	*current;

	current = tokens;
	while (current && (current->type == TOKEN_REDIR_IN
			|| current->type == TOKEN_REDIR_OUT
			|| current->type == TOKEN_REDIR_APPEND
			|| current->type == TOKEN_HEREDOC))
	{
		current = current->next;
		if (current && current->type == TOKEN_WORD)
			current = current->next;
	}
	return (current && (current->type == TOKEN_WORD
			|| current->type == TOKEN_VARIABLE));
}

t_ast_node	*parse_command(t_gc *gc, t_token **tokens, t_shell_env *env)
{
	t_ast_node			*cmd_node;
	char				**args;
	t_redirection		*input_redirs;
	t_redirection		*output_redirs;
	t_redirection_entry	*redirections;
	t_redir_params		params;
	int					redir_position;

	if (!*tokens || (!has_command_token(*tokens)
			&& !is_redirection_token((*tokens)->type)))
		return (NULL);
	cmd_node = ast_node_create(gc, NODE_CMD);
	if (!cmd_node)
		return (NULL);
	input_redirs = NULL;
	output_redirs = NULL;
	redirections = NULL;
	redir_position = 0;
	params.redirections = &redirections;
	params.redir_position = &redir_position;
	params.input_redirs = &input_redirs;
	params.output_redirs = &output_redirs;
	params.env = env;
	args = extract_args_with_redirections(gc, tokens, &params);
	cmd_node->args = args;
	cmd_node->redirections = redirections;
	cmd_node->input_redirs = input_redirs;
	cmd_node->output_redirs = output_redirs;
	return (cmd_node);
}
