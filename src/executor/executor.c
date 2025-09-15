/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexanfe <alexanfe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/07 01:36:37 by alexanfe          #+#    #+#             */
/*   Updated: 2025/09/15 13:16:03 by alexanfe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

static void	execute_child_process(char **args, char *command_path,
		t_redirection *input_redirs, t_redirection *output_redirs)
{
	if (setup_multiple_input_redirections(input_redirs) == -1)
	{
		free(command_path);
		exit(1);
	}
	if (setup_multiple_output_redirections(output_redirs) == -1)
	{
		free(command_path);
		exit(1);
	}
	if (execve(command_path, args, NULL) == -1)
	{
		ft_printf("minishell: %s: execution failed\n", args[0]);
		free(command_path);
		exit(127);
	}
}

static int	wait_for_child(pid_t pid)
{
	int	status;

	if (waitpid(pid, &status, 0) == -1)
		return (1);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	return (1);
}

int	execute_command(char **args)
{
	pid_t	pid;
	char	*command_path;

	if (!args || !args[0])
		return (1);
	command_path = resolve_command_path(args[0]);
	if (!command_path)
	{
		ft_printf("minishell: %s: command not found\n", args[0]);
		return (127);
	}
	pid = fork();
	if (pid == -1)
	{
		free(command_path);
		return (1);
	}
	if (pid == 0)
		execute_child_process(args, command_path, NULL, NULL);
	else
	{
		free(command_path);
		return (wait_for_child(pid));
	}
	return (0);
}

int	execute_command_with_redirections(t_ast_node *cmd_node)
{
	pid_t	pid;
	char	*command_path;

	command_path = get_validated_command_path(cmd_node);
	if (!command_path)
		return (127);
	pid = fork();
	if (pid == -1)
	{
		free(command_path);
		return (1);
	}
	if (pid == 0)
		execute_child_process(cmd_node->args, command_path,
			cmd_node->input_redirs, cmd_node->output_redirs);
	else
	{
		free(command_path);
		return (wait_for_child(pid));
	}
	return (0);
}

int	executor_execute(t_ast_node *ast)
{
	if (!ast)
		return (0);
	if (ast->type == NODE_CMD)
		return (execute_command_with_redirections(ast));
	else if (ast->type == NODE_PIPE)
		return (execute_pipe(ast));
	return (1);
}
