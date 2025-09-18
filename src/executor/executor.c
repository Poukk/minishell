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

static int	check_directory_and_fork(char *command_path, char **expanded_args,
		t_gc *gc)
{
	struct stat	path_stat;
	pid_t		pid;

	if (stat(command_path, &path_stat) == 0 && S_ISDIR(path_stat.st_mode))
	{
		ft_printf("minishell: %s: Is a directory\n", expanded_args[0]);
		free(command_path);
		gc_free_all(gc);
		return (126);
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

int	execute_command_with_redirections(t_ast_node *cmd_node, t_shell_env *env)
{
	pid_t		pid;
	t_cmd_setup	setup;
	t_gc		gc;
	int			result;

	gc_init(&gc);
	setup.gc = &gc;
	result = handle_command_setup(cmd_node, env, &setup);
	if (result != 0)
		return (result);
	pid = check_directory_and_fork(setup.command_path, setup.expanded_args,
			&gc);
	if (pid == 1 || pid == 126)
		return (pid);
	if (pid == 0)
		execute_child_process(setup.expanded_args, setup.command_path,
			cmd_node->input_redirs, cmd_node->output_redirs);
	else
	{
		free(setup.command_path);
		gc_free_all(&gc);
		return (wait_for_child(pid));
	}
	return (0);
}

int	executor_execute(t_ast_node *ast, t_shell_env *env)
{
	if (!ast)
		return (0);
	if (ast->type == NODE_CMD)
		return (execute_command_with_redirections(ast, env));
	else if (ast->type == NODE_PIPE)
		return (execute_pipe(ast, env));
	return (1);
}
