/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_builtins.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexanfe <alexanfe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/19 02:15:00 by alexanfe          #+#    #+#             */
/*   Updated: 2025/09/19 02:15:01 by alexanfe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	restore_saved_fds(int saved_fds[2])
{
	dup2(saved_fds[0], STDIN_FILENO);
	dup2(saved_fds[1], STDOUT_FILENO);
	close(saved_fds[0]);
	close(saved_fds[1]);
}

static int	setup_builtin_redirections(t_ast_node *cmd_node, int saved_fds[2])
{
	saved_fds[0] = dup(STDIN_FILENO);
	saved_fds[1] = dup(STDOUT_FILENO);
	if (setup_multiple_in_redirections(cmd_node->input_redirs) == -1
		|| setup_multiple_out_redirections(cmd_node->output_redirs) == -1)
	{
		restore_saved_fds(saved_fds);
		return (-1);
	}
	return (0);
}

int	execute_builtin_with_redirections(t_ast_node *cmd_node,
		t_shell_context *ctx)
{
	t_cmd_setup		setup;
	t_gc			gc;
	int				result;
	int				saved_fds[2];

	gc_init(&gc);
	setup.gc = &gc;
	result = handle_command_setup(cmd_node, ctx->env, &setup);
	if (result != 0)
	{
		gc_free_all(&gc);
		return (result);
	}
	if (setup_builtin_redirections(cmd_node, saved_fds) == -1)
	{
		gc_free_all(&gc);
		return (1);
	}
	result = execute_builtin(is_builtin_command(setup.expanded_args[0]),
			setup.expanded_args, ctx);
	restore_saved_fds(saved_fds);
	gc_free_all(&gc);
	return (result);
}
