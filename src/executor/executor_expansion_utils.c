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

static char	*get_variable_value(t_gc *gc, char *var_name, t_shell_env *env)
{
	char	*var_value;

	if (ft_strncmp(var_name, "?", 1) == 0)
	{
		var_value = expand_variable(gc, var_name, env);
	}
	else
	{
		var_value = env_get_value(env, var_name);
		if (!var_value)
			var_value = "";
	}
	return (var_value);
}

char	*process_variable_char(t_gc *gc, char **current, t_shell_env *env,
		char *result)
{
	char	*var_name;
	char	*var_value;

	var_name = extract_var_name(gc, *current + 1);
	if (var_name)
	{
		var_value = get_variable_value(gc, var_name, env);
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
	if (*end == '?')
	{
		end++;
	}
	else
	{
		while (*end && (ft_isalnum(*end) || *end == '_'))
			end++;
	}
	len = end - start;
	if (len == 0)
		return (NULL);
	var_name = gc_malloc(gc, len + 1);
	if (!var_name)
		return (NULL);
	ft_strlcpy(var_name, start, len + 1);
	return (var_name);
}
