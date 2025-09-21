/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexanfe <alexanfe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/07 01:36:37 by alexanfe          #+#    #+#             */
/*   Updated: 2025/12/19 12:00:00 by alexanfe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	handle_external_child_process(t_external_exec_data *data, pid_t pid)
{
	t_child_exec_ctx	exec_ctx;

	if (pid == 0)
	{
		setup_child_exec_context(&exec_ctx, data->cmd_node, data->ctx);
		execute_child_process(data->setup->exp_args,
			data->setup->command_path, &exec_ctx);
	}
	else
	{
		free(data->setup->command_path);
		gc_free_all(data->gc);
		return (wait_for_child(pid));
	}
	return (0);
}

static int	execute_external_command(t_cmd_setup *setup, t_ast_node *cmd_node,
		t_gc *gc, t_shell_context *ctx)
{
	t_external_exec_data	data;
	pid_t					pid;

	setup_command_signals();
	pid = check_directory_and_fork(setup->command_path, setup->exp_args,
			gc);
	if (pid == 1 || pid == 126)
	{
		setup_shell_signals();
		return (pid);
	}
	data.setup = setup;
	data.cmd_node = cmd_node;
	data.gc = gc;
	data.ctx = ctx;
	return (handle_external_child_process(&data, pid));
}

static int	execute_builtin_command(t_cmd_setup *setup, t_ast_node *cmd_node,
		t_shell_context *ctx, t_gc *gc)
{
	int	result;

	result = execute_builtin_with_redirections_expanded(setup,
			cmd_node, ctx);
	gc_free_all(gc);
	return (result);
}

int	execute_cmd(t_ast_node *cmd_node, t_shell_context *ctx)
{
	t_cmd_setup	setup;
	t_gc		gc;
	int			result;

	result = setup_and_validate_command(cmd_node, ctx->env, &setup, &gc);
	if (result == 2)
		return (0);
	if (result != 0)
		return (result);
	if (is_builtin_command(setup.exp_args[0]) != BUILTIN_NONE)
		return (execute_builtin_command(&setup, cmd_node, ctx, &gc));
	result = execute_external_command(&setup, cmd_node, &gc, ctx);
	setup_shell_signals();
	return (result);
}

int	execute(t_ast_node *ast, t_shell_context *ctx)
{
	if (!ast)
		return (0);
	if (ast->type == NODE_CMD)
		return (execute_cmd(ast, ctx));
	else if (ast->type == NODE_PIPE)
		return (execute_pipe(ast, ctx));
	return (1);
}
