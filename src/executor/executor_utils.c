/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexanfe <alexanfe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 12:00:00 by alexanfe          #+#    #+#             */
/*   Updated: 2025/09/18 12:00:00 by alexanfe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <errno.h>

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
		t_redirection *input_redirs, t_redirection *output_redirs)
{
	setup_command_signals();
	if (setup_multiple_in_redirections(input_redirs) == -1)
	{
		free(command_path);
		exit(1);
	}
	if (setup_multiple_out_redirections(output_redirs) == -1)
	{
		free(command_path);
		exit(1);
	}
	if (execve(command_path, args, NULL) == -1)
		handle_execve_error(args, command_path);
}
