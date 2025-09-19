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
	setup->expanded_args = expand_command_args(setup->gc, cmd_node->args, env);
	if (!setup->expanded_args)
	{
		gc_free_all(setup->gc);
		return (1);
	}
	return (0);
}

static int	resolve_command(t_cmd_setup *setup)
{
	setup->command_path = resolve_command_path(setup->expanded_args[0]);
	if (!setup->command_path)
	{
		ft_dprintf(2, "minishell: %s: command not found\n",
			setup->expanded_args[0]);
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
	builtin_type = is_builtin_command(setup->expanded_args[0]);
	if (builtin_type != BUILTIN_NONE)
	{
		setup->command_path = NULL;
		return (0);
	}
	return (resolve_command(setup));
}
