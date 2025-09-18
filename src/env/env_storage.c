/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_storage.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexanfe <alexanfe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 19:47:43 by alexanfe          #+#    #+#             */
/*   Updated: 2025/09/18 19:47:44 by alexanfe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_shell_env	*env_create(t_gc *gc)
{
	t_shell_env	*env;

	env = gc_malloc(gc, sizeof(t_shell_env));
	if (!env)
		return (NULL);
	env->vars = NULL;
	env->last_exit_code = 0;
	env->cwd = NULL;
	return (env);
}

t_env_var	*env_find_var(t_shell_env *env, const char *key)
{
	t_env_var	*current;

	if (!env || !key)
		return (NULL);
	current = env->vars;
	while (current)
	{
		if (ft_strncmp(current->key, key, ft_strlen(key)) == 0
			&& ft_strlen(current->key) == ft_strlen(key))
			return (current);
		current = current->next;
	}
	return (NULL);
}

static t_env_var	*create_env_var(t_gc *gc,
		const char *key, const char *value)
{
	t_env_var	*var;

	var = gc_malloc(gc, sizeof(t_env_var));
	if (!var)
		return (NULL);
	var->key = gc_malloc(gc, ft_strlen(key) + 1);
	if (!var->key)
		return (NULL);
	var->value = gc_malloc(gc, ft_strlen(value) + 1);
	if (!var->value)
		return (NULL);
	ft_strlcpy(var->key, key, ft_strlen(key) + 1);
	ft_strlcpy(var->value, value, ft_strlen(value) + 1);
	var->next = NULL;
	return (var);
}

int	env_set_var(t_gc *gc, t_shell_env *env, const char *key, const char *value)
{
	t_env_var	*var;
	t_env_var	*new_var;

	if (!env || !key || !value)
		return (-1);
	var = env_find_var(env, key);
	if (var)
	{
		var->value = gc_malloc(gc, ft_strlen(value) + 1);
		if (!var->value)
			return (-1);
		ft_strlcpy(var->value, value, ft_strlen(value) + 1);
		return (0);
	}
	new_var = create_env_var(gc, key, value);
	if (!new_var)
		return (-1);
	new_var->next = env->vars;
	env->vars = new_var;
	return (0);
}

int	env_unset_var(t_shell_env *env, const char *key)
{
	t_env_var	*current;
	t_env_var	*prev;

	if (!env || !key)
		return (-1);
	current = env->vars;
	prev = NULL;
	while (current)
	{
		if (ft_strncmp(current->key, key, ft_strlen(key)) == 0
			&& ft_strlen(current->key) == ft_strlen(key))
		{
			if (prev)
				prev->next = current->next;
			else
				env->vars = current->next;
			return (0);
		}
		prev = current;
		current = current->next;
	}
	return (-1);
}
