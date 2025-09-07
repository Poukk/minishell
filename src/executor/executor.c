/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexanfe <alexanfe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/07 01:36:37 by alexanfe          #+#    #+#             */
/*   Updated: 2025/09/07 01:36:38 by alexanfe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

static void	execute_child_process(char **args, char *command_path)
{
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
		execute_child_process(args, command_path);
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
		return (execute_command(ast->args));
	else if (ast->type == NODE_PIPE)
	{
		ft_printf("Error: Pipe execution not implemented yet (Phase 4)\n");
		return (1);
	}
	return (1);
}
