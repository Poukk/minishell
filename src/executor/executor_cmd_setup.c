/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_cmd_setup.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexanfe <alexanfe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/19 17:46:59 by alexanfe          #+#    #+#             */
/*   Updated: 2025/09/19 17:47:00 by alexanfe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	setup_command_args(t_ast_node *cmd_node, t_shell_env *env,
		t_cmd_setup *setup)
{
	setup->exp_args = expand_command_args(setup->gc, cmd_node->args, env);
	if (!setup->exp_args)
	{
		gc_free_all(setup->gc);
		return (1);
	}
	if (!setup->exp_args[0])
	{
		return (2);
	}
	return (0);
}

static int	resolve_command(t_cmd_setup *setup, t_shell_env *env)
{
	struct stat	path_stat;

	setup->command_path = resolve_command_path(setup->exp_args[0], env);
	if (!setup->command_path)
	{
		if ((setup->exp_args[0][0] == '/'
			|| (setup->exp_args[0][0] == '.' && setup->exp_args[0][1] == '/')
			|| (setup->exp_args[0][0] == '.' && setup->exp_args[0][1] == '.'
			&& setup->exp_args[0][2] == '/')) &&
			access(setup->exp_args[0], F_OK) == 0
			&& stat(setup->exp_args[0], &path_stat) == 0
			&& S_ISDIR(path_stat.st_mode))
		{
			ft_dprintf(2, "minishell: %s: Is a directory\n",
				setup->exp_args[0]);
			gc_free_all(setup->gc);
			return (126);
		}
		ft_dprintf(2, "minishell: %s: command not found\n",
			setup->exp_args[0]);
		gc_free_all(setup->gc);
		return (127);
	}
	return (0);
}

int	handle_command_setup(t_ast_node *cmd_node, t_shell_env *env,
		t_cmd_setup *setup)
{
	t_builtin_type	builtin_type;
	int				result;

	result = setup_command_args(cmd_node, env, setup);
	if (result != 0)
		return (result);
	builtin_type = is_builtin_command(setup->exp_args[0]);
	if (builtin_type != BUILTIN_NONE)
	{
		setup->command_path = NULL;
		return (0);
	}
	return (resolve_command(setup, env));
}

int	setup_and_validate_command(t_ast_node *cmd_node, t_shell_env *env,
		t_cmd_setup *setup, t_gc *gc)
{
	int	result;

	if (!cmd_node || !cmd_node->args || !cmd_node->args[0])
		return (1);
	gc_init(gc);
	setup->gc = gc;
	result = handle_command_setup(cmd_node, env, setup);
	if (result == 2)
	{
		gc_free_all(gc);
		return (2);
	}
	return (result);
}

void	setup_child_exec_context(t_child_exec_ctx *exec_ctx,
		t_ast_node *cmd_node, t_shell_context *ctx)
{
	exec_ctx->redirections = cmd_node->redirections;
	exec_ctx->input_redirs = cmd_node->input_redirs;
	exec_ctx->output_redirs = cmd_node->output_redirs;
	exec_ctx->env = ctx->env;
}
