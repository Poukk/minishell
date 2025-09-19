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
