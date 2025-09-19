/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_vars.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexanfe <alexanfe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/19 17:47:46 by alexanfe          #+#    #+#             */
/*   Updated: 2025/09/19 17:47:47 by alexanfe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_delimiter_quoted(char *delimiter)
{
	size_t	len;

	len = ft_strlen(delimiter);
	if (len < 2)
		return (0);
	return ((delimiter[0] == '\'' && delimiter[len - 1] == '\'')
		|| (delimiter[0] == '"' && delimiter[len - 1] == '"'));
}

static char	*expand_single_variable(t_gc *gc, char *var_name)
{
	char	*env_value;
	char	*result;

	env_value = getenv(var_name);
	if (!env_value)
		return (gc_malloc(gc, 1));
	result = gc_malloc(gc, ft_strlen(env_value) + 1);
	if (!result)
		return (NULL);
	ft_strlcpy(result, env_value, ft_strlen(env_value) + 1);
	return (result);
}

char	*process_variable_expansion(t_gc *gc, char *content, size_t *i)
{
	size_t	start;
	size_t	end;
	char	*var_name;
	char	*var_value;

	(*i)++;
	start = *i;
	while (content[*i] && (ft_isalnum(content[*i]) || content[*i] == '_'))
		(*i)++;
	end = *i;
	if (start == end)
		return (gc_malloc(gc, 1));
	var_name = gc_malloc(gc, end - start + 1);
	if (!var_name)
		return (NULL);
	ft_strlcpy(var_name, content + start, end - start + 1);
	var_value = expand_single_variable(gc, var_name);
	(*i)--;
	return (var_value);
}

int	should_expand_variable(char *content, size_t i)
{
	if (content[i] != '$')
		return (0);
	if (content[i + 1] == '$')
		return (0);
	return (content[i + 1] && (ft_isalpha(content[i + 1])
			|| content[i + 1] == '_'));
}

char	*expand_heredoc_variables(t_gc *gc, char *content, char *delimiter)
{
	char	*result;

	if (!content)
		return (NULL);
	if (is_delimiter_quoted(delimiter))
		return (content);
	result = init_expansion_result(gc);
	if (!result)
		return (NULL);
	result = process_expansion_loop(gc, content, result);
	return (result);
}
