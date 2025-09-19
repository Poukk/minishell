/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexanfe <alexanfe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 19:47:52 by alexanfe          #+#    #+#             */
/*   Updated: 2025/09/19 17:01:12 by alexanfe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	count_env_vars(t_shell_env *env)
{
	t_env_var	*current;
	int			count;

	if (!env)
		return (0);
	count = 0;
	current = env->vars;
	while (current)
	{
		count++;
		current = current->next;
	}
	return (count);
}

static char	*create_env_string(t_gc *gc, t_env_var *var)
{
	char	*env_str;
	int		key_len;
	int		total_len;

	key_len = ft_strlen(var->key);
	total_len = key_len + ft_strlen(var->value) + 2;
	env_str = gc_malloc(gc, total_len);
	if (!env_str)
		return (NULL);
	ft_strlcpy(env_str, var->key, key_len + 1);
	ft_strlcat(env_str, "=", key_len + 2);
	ft_strlcat(env_str, var->value, total_len);
	return (env_str);
}

char	**env_to_array(t_gc *gc, t_shell_env *env)
{
	char		**env_array;
	t_env_var	*current;
	int			count;
	int			i;

	count = count_env_vars(env);
	env_array = gc_malloc(gc, sizeof(char *) * (count + 1));
	if (!env_array)
		return (NULL);
	current = env->vars;
	i = 0;
	while (current && i < count)
	{
		env_array[i] = create_env_string(gc, current);
		if (!env_array[i])
			return (NULL);
		current = current->next;
		i++;
	}
	env_array[i] = NULL;
	return (env_array);
}

int	env_is_valid_name(const char *name)
{
	int	i;

	if (!name || !*name)
		return (0);
	if (!ft_isalpha(name[0]) && name[0] != '_')
		return (0);
	i = 1;
	while (name[i])
	{
		if (!ft_isalnum(name[i]) && name[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

char	*env_get_value(t_shell_env *env, const char *key)
{
	t_env_var	*var;

	var = env_find_var(env, key);
	if (!var)
		return (NULL);
	return (var->value);
}
