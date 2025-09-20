/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexanfe <alexanfe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/19 17:47:17 by alexanfe          #+#    #+#             */
/*   Updated: 2025/09/19 17:47:22 by alexanfe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <errno.h>

void	init_empty_exec_ctx(t_child_exec_ctx *ctx)
{
	ctx->redirections = NULL;
	ctx->input_redirs = NULL;
	ctx->output_redirs = NULL;
	ctx->env = NULL;
}

void	execute_simple_command_child(char **args, char *command_path)
{
	t_child_exec_ctx	ctx;

	init_empty_exec_ctx(&ctx);
	execute_child_process(args, command_path, &ctx);
}

void	handle_execve_error(char **args, char *command_path)
{
	if (errno == EACCES)
	{
		print_command_error(args[0], "Permission denied");
		free(command_path);
		exit(EXIT_EXEC_FAILED);
	}
	else if (errno == EISDIR)
	{
		print_command_error(args[0], "Is a directory");
		free(command_path);
		exit(EXIT_EXEC_FAILED);
	}
	else
	{
		print_command_error(args[0], "execution failed");
		free(command_path);
		exit(EXIT_CMD_NOT_FOUND);
	}
}

int	wait_for_child(pid_t pid)
{
	int	status;

	if (waitpid(pid, &status, 0) == -1)
		return (1);
	return (process_child_status(status));
}

void	execute_child_process(char **args, char *command_path,
		t_child_exec_ctx *ctx)
{
	t_gc	gc;
	char	**env_array;

	gc_init(&gc);
	setup_command_signals();
	if (ctx->redirections)
	{
		if (setup_redirections_ordered(ctx->redirections) == -1)
		{
			free(command_path);
			exit(1);
		}
	}
	else
	{
		if (setup_multiple_in_redirections(ctx->input_redirs) == -1)
		{
			free(command_path);
			exit(1);
		}
		if (setup_multiple_out_redirections(ctx->output_redirs) == -1)
		{
			free(command_path);
			exit(1);
		}
	}
	env_array = NULL;
	if (ctx->env)
		env_array = env_to_array(&gc, ctx->env);
	if (execve(command_path, args, env_array) == -1)
		handle_execve_error(args, command_path);
	gc_free_all(&gc);
}
