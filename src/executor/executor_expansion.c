/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_expansion.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexanfe <alexanfe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 12:30:00 by alexanfe          #+#    #+#             */
/*   Updated: 2025/09/18 12:30:00 by alexanfe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

int	count_args(char **args)
{
	int	count;

	count = 0;
	while (args[count])
		count++;
	return (count);
}

char	*process_single_arg(t_gc *gc, char *arg, t_shell_env *env)
{
	char	*result;

	if (arg[0] == '$')
		result = expand_variable(gc, arg + 1, env);
	else
	{
		result = gc_malloc(gc, ft_strlen(arg) + 1);
		if (result)
			ft_strlcpy(result, arg, ft_strlen(arg) + 1);
	}
	return (result);
}

char	**expand_command_args(t_gc *gc, char **args, t_shell_env *env)
{
	char	**expanded_args;
	int		i;
	int		count;

	if (!args)
		return (NULL);
	count = count_args(args);
	expanded_args = gc_malloc(gc, sizeof(char *) * (count + 1));
	if (!expanded_args)
		return (NULL);
	i = 0;
	while (args[i])
	{
		expanded_args[i] = process_single_arg(gc, args[i], env);
		if (!expanded_args[i])
			return (NULL);
		i++;
	}
	expanded_args[i] = NULL;
	return (expanded_args);
}

int	handle_command_setup(t_ast_node *cmd_node, t_shell_env *env,
		t_cmd_setup *setup)
{
	setup->expanded_args = expand_command_args(setup->gc, cmd_node->args, env);
	if (!setup->expanded_args)
	{
		gc_free_all(setup->gc);
		return (1);
	}
	setup->command_path = resolve_command_path(setup->expanded_args[0]);
	if (!setup->command_path)
	{
		ft_printf("minishell: %s: command not found\n",
			setup->expanded_args[0]);
		gc_free_all(setup->gc);
		return (127);
	}
	return (0);
}
