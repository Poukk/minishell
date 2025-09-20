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
		if (setup_redir_ordered(ctx->redirections) == -1)
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
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	setup_child_redirections(ctx, command_path);
	env_array = NULL;
	if (ctx->env)
		env_array = env_to_array(&gc, ctx->env);
	if (execve(command_path, args, env_array) == -1)
		handle_execve_error(args, command_path);
	gc_free_all(&gc);
}

int	check_directory_and_fork(char *command_path, char **expanded_args,
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

int	wait_for_pipe_children(pid_t left_pid, pid_t right_pid,
			int pipefd[2])
{
	int	left_status;
	int	right_status;

	close(pipefd[0]);
	close(pipefd[1]);
	waitpid(left_pid, &left_status, 0);
	waitpid(right_pid, &right_status, 0);
	reset_signal_received();
	if (WIFEXITED(right_status))
		return (WEXITSTATUS(right_status));
	return (1);
}

int	setup_pipe_and_signals(int pipefd[2])
{
	if (pipe(pipefd) == -1)
		return (1);
	setup_command_signals();
	return (0);
}
