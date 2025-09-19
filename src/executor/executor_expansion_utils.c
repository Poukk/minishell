/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_expansion_utils.c                        :+:      :+:    :+:   */
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

char	*process_variable_char(t_gc *gc, char **current, t_shell_env *env,
		char *result)
{
	char	*var_name;
	char	*var_value;

	var_name = extract_var_name(gc, *current + 1);
	if (var_name)
	{
		var_value = env_get_value(env, var_name);
		if (!var_value)
			var_value = "";
		result = append_to_result(gc, result, var_value);
		if (!result)
			return (NULL);
		*current += ft_strlen(var_name) + 1;
	}
	else
	{
		result = append_to_result(gc, result, "$");
		if (!result)
			return (NULL);
		(*current)++;
	}
	return (result);
}

char	*process_regular_char(t_gc *gc, char **current, char *result)
{
	char	single_char[2];

	single_char[0] = **current;
	single_char[1] = '\0';
	result = append_to_result(gc, result, single_char);
	if (!result)
		return (NULL);
	(*current)++;
	return (result);
}

char	*append_to_result(t_gc *gc, char *result, char *append_str)
{
	char	*new_result;
	int		total_len;

	if (!append_str || !*append_str)
		return (result);
	total_len = ft_strlen(result) + ft_strlen(append_str) + 1;
	new_result = gc_malloc(gc, total_len);
	if (!new_result)
		return (NULL);
	ft_strlcpy(new_result, result, ft_strlen(result) + 1);
	ft_strlcat(new_result, append_str, total_len);
	return (new_result);
}

char	*extract_var_name(t_gc *gc, char *start)
{
	char	*end;
	int		len;
	char	*var_name;

	end = start;
	while (*end && (ft_isalnum(*end) || *end == '_'))
		end++;
	len = end - start;
	if (len == 0)
		return (NULL);
	var_name = gc_malloc(gc, len + 1);
	if (!var_name)
		return (NULL);
	ft_strlcpy(var_name, start, len + 1);
	return (var_name);
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
