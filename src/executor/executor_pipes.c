/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_pipes.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexanfe <alexanfe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 13:15:55 by alexanfe          #+#    #+#             */
/*   Updated: 2025/09/10 13:15:56 by alexanfe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

static void	execute_left_command(t_ast_node *node, int pipefd[2],
		t_shell_context *ctx)
{
	close(pipefd[0]);
	if (dup2(pipefd[1], STDOUT_FILENO) == -1)
		exit(1);
	close(pipefd[1]);
	exit(executor_execute(node, ctx));
}

static void	execute_right_command(t_ast_node *node, int pipefd[2],
		t_shell_context *ctx)
{
	close(pipefd[1]);
	if (dup2(pipefd[0], STDIN_FILENO) == -1)
		exit(1);
	close(pipefd[0]);
	exit(executor_execute(node, ctx));
}

static int	wait_for_pipe_children(pid_t left_pid, pid_t right_pid,
			int pipefd[2])
{
	int	left_status;
	int	right_status;

	close(pipefd[0]);
	close(pipefd[1]);
	waitpid(left_pid, &left_status, 0);
	waitpid(right_pid, &right_status, 0);
	if (WIFEXITED(right_status))
		return (WEXITSTATUS(right_status));
	return (1);
}

int	execute_pipe(t_ast_node *ast, t_shell_context *ctx)
{
	int		pipefd[2];
	pid_t	left_pid;
	pid_t	right_pid;

	if (pipe(pipefd) == -1)
		return (1);
	left_pid = fork();
	if (left_pid == -1)
	{
		close(pipefd[0]);
		close(pipefd[1]);
		return (1);
	}
	if (left_pid == 0)
		execute_left_command(ast->left, pipefd, ctx);
	right_pid = fork();
	if (right_pid == -1)
	{
		close(pipefd[0]);
		close(pipefd[1]);
		return (1);
	}
	if (right_pid == 0)
		execute_right_command(ast->right, pipefd, ctx);
	return (wait_for_pipe_children(left_pid, right_pid, pipefd));
}
