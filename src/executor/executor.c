/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexanfe <alexanfe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/07 01:36:37 by alexanfe          #+#    #+#             */
/*   Updated: 2025/09/19 17:01:57 by alexanfe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	execute_command(char **args, t_shell_env *env)
{
	pid_t	pid;
	char	*command_path;

	if (!args || !args[0])
		return (1);
	command_path = resolve_command_path(args[0], env);
	if (!command_path)
		return (return_error_code(EXIT_CMD_NOT_FOUND, args[0], NULL,
				"command not found"));
	pid = fork();
	if (pid == -1)
	{
		free(command_path);
		return (1);
	}
	if (pid == 0)
		execute_simple_command_child(args, command_path);
	else
	{
		free(command_path);
		return (wait_for_child(pid));
	}
	return (0);
}

static int	check_directory_and_fork(char *command_path, char **expanded_args,
		t_gc *gc)
{
	struct stat	path_stat;
	pid_t		pid;

	if (stat(command_path, &path_stat) == 0 && S_ISDIR(path_stat.st_mode))
	{
		free(command_path);
		gc_free_all(gc);
		return (return_error_code(EXIT_EXEC_FAILED, expanded_args[0], NULL,
				"Is a directory"));
	}
	pid = fork();
	if (pid == -1)
	{
		free(command_path);
		gc_free_all(gc);
		return (1);
	}
	return (pid);
}

static int	execute_external_command(t_cmd_setup *setup, t_ast_node *cmd_node,
		t_gc *gc, t_shell_context *ctx)
{
	t_child_exec_ctx	exec_ctx;
	pid_t				pid;

	pid = check_directory_and_fork(setup->command_path, setup->exp_args,
			gc);
	if (pid == 1 || pid == 126)
		return (pid);
	if (pid == 0)
	{
		exec_ctx.redirections = cmd_node->redirections;
		exec_ctx.input_redirs = cmd_node->input_redirs;
		exec_ctx.output_redirs = cmd_node->output_redirs;
		exec_ctx.env = ctx->env;
		execute_child_process(setup->exp_args,
			setup->command_path, &exec_ctx);
	}
	else
	{
		free(setup->command_path);
		gc_free_all(gc);
		return (wait_for_child(pid));
	}
	return (0);
}

int	execute_command_with_redirections(t_ast_node *cmd_node,
		t_shell_context *ctx)
{
	t_cmd_setup	setup;
	t_gc		gc;
	int			result;

	if (!cmd_node || !cmd_node->args || !cmd_node->args[0])
		return (1);
	gc_init(&gc);
	setup.gc = &gc;
	result = handle_command_setup(cmd_node, ctx->env, &setup);
	if (result == 2)
	{
		gc_free_all(&gc);
		return (0);
	}
	if (result != 0)
		return (result);
	if (is_builtin_command(setup.exp_args[0]) != BUILTIN_NONE)
	{
		result = execute_builtin_with_redirections_expanded(&setup,
				cmd_node, ctx);
		gc_free_all(&gc);
		return (result);
	}
	return (execute_external_command(&setup, cmd_node, &gc, ctx));
}

int	execute(t_ast_node *ast, t_shell_context *ctx)
{
	if (!ast)
		return (0);
	if (ast->type == NODE_CMD)
		return (execute_command_with_redirections(ast, ctx));
	else if (ast->type == NODE_PIPE)
		return (execute_pipe(ast, ctx));
	return (1);
}
