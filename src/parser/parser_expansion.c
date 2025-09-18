/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_expansion.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexanfe <alexanfe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 01:22:42 by alexanfe          #+#    #+#             */
/*   Updated: 2025/09/15 01:22:43 by alexanfe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*handle_exit_code_expansion(t_gc *gc, t_shell_env *env)
{
	char	*value;
	char	*result;
	int		exit_code;

	exit_code = 0;
	if (env)
		exit_code = env->last_exit_code;
	result = ft_itoa(exit_code);
	if (!result)
		return (NULL);
	value = gc_malloc(gc, ft_strlen(result) + 1);
	if (!value)
	{
		free(result);
		return (NULL);
	}
	ft_strlcpy(value, result, ft_strlen(result) + 1);
	free(result);
	return (value);
}

static char	*handle_regular_expansion(t_gc *gc, const char *var_name,
		t_shell_env *env)
{
	char	*value;
	char	*result;

	value = NULL;
	if (env)
		value = env_get_value(env, var_name);
	if (!value)
	{
		result = gc_malloc(gc, 1);
		if (result)
			result[0] = '\0';
		return (result);
	}
	result = gc_malloc(gc, ft_strlen(value) + 1);
	if (!result)
		return (NULL);
	ft_strlcpy(result, value, ft_strlen(value) + 1);
	return (result);
}

char	*expand_variable(t_gc *gc, const char *var_name, t_shell_env *env)
{
	if (ft_strncmp(var_name, "?", 1) == 0)
		return (handle_exit_code_expansion(gc, env));
	if (var_name[0] == '$')
		var_name++;
	return (handle_regular_expansion(gc, var_name, env));
}
