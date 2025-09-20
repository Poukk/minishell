/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_pipes.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexanfe <alexanfe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 13:15:55 by alexanfe          #+#    #+#             */
/*   Updated: 2025/09/19 17:47:11 by alexanfe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	execute_left_command(t_ast_node *node, int pipefd[2],
		t_shell_context *ctx)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	close(pipefd[0]);
	if (dup2(pipefd[1], STDOUT_FILENO) == -1)
		exit(1);
	close(pipefd[1]);
	exit(execute(node, ctx));
}

static void	execute_right_command(t_ast_node *node, int pipefd[2],
		t_shell_context *ctx)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	close(pipefd[1]);
	if (dup2(pipefd[0], STDIN_FILENO) == -1)
		exit(1);
	close(pipefd[0]);
	exit(execute(node, ctx));
}

static int	fork_left_child(t_ast_node *ast, int pipefd[2],
	t_shell_context *ctx)
{
	pid_t	left_pid;

	left_pid = fork();
	if (left_pid == -1)
	{
		close(pipefd[0]);
		close(pipefd[1]);
		setup_shell_signals();
		return (-1);
	}
	if (left_pid == 0)
		execute_left_command(ast->left, pipefd, ctx);
	return (left_pid);
}

static int	fork_right_child(t_ast_node *ast, int pipefd[2],
	t_shell_context *ctx)
{
	pid_t	right_pid;

	right_pid = fork();
	if (right_pid == -1)
	{
		close(pipefd[0]);
		close(pipefd[1]);
		setup_shell_signals();
		return (-1);
	}
	if (right_pid == 0)
		execute_right_command(ast->right, pipefd, ctx);
	return (right_pid);
}

int	execute_pipe(t_ast_node *ast, t_shell_context *ctx)
{
	int		pipefd[2];
	pid_t	left_pid;
	pid_t	right_pid;
	int		result;

	if (setup_pipe_and_signals(pipefd) == 1)
		return (1);
	left_pid = fork_left_child(ast, pipefd, ctx);
	if (left_pid == -1)
		return (1);
	right_pid = fork_right_child(ast, pipefd, ctx);
	if (right_pid == -1)
		return (1);
	result = wait_for_pipe_children(left_pid, right_pid, pipefd);
	setup_shell_signals();
	return (result);
}
