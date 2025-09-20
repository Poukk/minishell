/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexanfe <alexanfe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/19 17:47:31 by alexanfe          #+#    #+#             */
/*   Updated: 2025/09/19 17:47:32 by alexanfe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	count_args(char **args)
{
	int	count;

	count = 0;
	while (args[count])
		count++;
	return (count);
}

char	*expand_variable_in_string(t_gc *gc, char *str, t_shell_env *env)
{
	char	*result;
	char	*current;

	if (!str)
		return (NULL);
	result = gc_malloc(gc, 1);
	if (!result)
		return (NULL);
	result[0] = '\0';
	current = str;
	while (*current)
	{
		if (*current == '$' && *(current + 1)
			&& (ft_isalnum(*(current + 1)) || *(current + 1) == '_'
				|| *(current + 1) == '?'))
			result = process_variable_char(gc, &current, env, result);
		else
			result = process_regular_char(gc, &current, result);
		if (!result)
			return (NULL);
	}
	return (result);
}

char	*process_single_arg(t_gc *gc, char *arg, t_shell_env *env)
{
	char	*result;
	char	*actual_content;

	if (!arg)
		return (NULL);
	if (ft_strncmp(arg, "SINGLE_QUOTE:", 13) == 0)
	{
		actual_content = arg + 13;
		result = gc_malloc(gc, ft_strlen(actual_content) + 1);
		if (result)
			ft_strlcpy(result, actual_content, ft_strlen(actual_content) + 1);
		return (result);
	}
	else if (ft_strncmp(arg, "DOUBLE_QUOTE:", 13) == 0)
	{
		actual_content = arg + 13;
		return (expand_variable_in_string(gc, actual_content, env));
	}
	else
	{
		return (expand_variable_in_string(gc, arg, env));
	}
}

static char	**expand_and_count_args(t_gc *gc, char **args, t_shell_env *env,
		int *final_count)
{
	char	**temp_args;
	int		i;
	int		count;

	count = count_args(args);
	temp_args = gc_malloc(gc, sizeof(char *) * (count + 1));
	if (!temp_args)
		return (NULL);
	i = 0;
	*final_count = 0;
	while (args[i])
	{
		temp_args[i] = process_single_arg(gc, args[i], env);
		if (!temp_args[i])
			return (NULL);
		if (temp_args[i][0] != '\0')
			(*final_count)++;
		i++;
	}
	temp_args[i] = NULL;
	return (temp_args);
}

char	**expand_command_args(t_gc *gc, char **args, t_shell_env *env)
{
	char	**temp_args;
	char	**expanded_args;
	int		final_count;
	int		i;

	if (!args)
		return (NULL);
	temp_args = expand_and_count_args(gc, args, env, &final_count);
	if (!temp_args)
		return (NULL);
	expanded_args = gc_malloc(gc, sizeof(char *) * (final_count + 1));
	if (!expanded_args)
		return (NULL);
	i = 0;
	final_count = 0;
	while (temp_args[i])
	{
		if (temp_args[i][0] != '\0')
			expanded_args[final_count++] = temp_args[i];
		i++;
	}
	expanded_args[final_count] = NULL;
	return (expanded_args);
}
