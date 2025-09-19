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

void	handle_execve_error(char **args, char *command_path)
{
	if (errno == EACCES)
	{
		ft_printf("minishell: %s: Permission denied\n", args[0]);
		free(command_path);
		exit(126);
	}
	else if (errno == EISDIR)
	{
		ft_printf("minishell: %s: Is a directory\n", args[0]);
		free(command_path);
		exit(126);
	}
	else
	{
		ft_printf("minishell: %s: execution failed\n", args[0]);
		free(command_path);
		exit(127);
	}
}

int	wait_for_child(pid_t pid)
{
	int	status;

	if (waitpid(pid, &status, 0) == -1)
		return (1);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	return (1);
}

void	execute_child_process(char **args, char *command_path,
		t_redirection *input_redirs, t_redirection *output_redirs)
{
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
