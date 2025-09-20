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

static int	setup_child_redirections(t_child_exec_ctx *ctx, char *command_path)
{
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
	return (0);
}

void	execute_child_process(char **args, char *command_path,
		t_child_exec_ctx *ctx)
{
	t_gc	gc;
	char	**env_array;

	gc_init(&gc);
	setup_command_signals();
	setup_child_redirections(ctx, command_path);
	env_array = NULL;
	if (ctx->env)
		env_array = env_to_array(&gc, ctx->env);
	if (execve(command_path, args, env_array) == -1)
		handle_execve_error(args, command_path);
	gc_free_all(&gc);
}
