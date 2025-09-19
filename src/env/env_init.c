/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_init.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexanfe <alexanfe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 19:48:04 by alexanfe          #+#    #+#             */
/*   Updated: 2025/09/19 17:01:05 by alexanfe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <unistd.h>

static int	parse_env_string(const char *env_str, char **key, char **value)
{
	char	*equal_sign;
	size_t	key_len;
	size_t	value_len;

	equal_sign = ft_strchr(env_str, '=');
	if (!equal_sign)
		return (-1);
	key_len = equal_sign - env_str;
	value_len = ft_strlen(equal_sign + 1);
	*key = ft_substr(env_str, 0, key_len);
	if (!*key)
		return (-1);
	*value = ft_substr(env_str, key_len + 1, value_len);
	if (!*value)
		return (-1);
	return (0);
}

t_shell_env	*env_init_from_system(t_gc *gc, char **envp)
{
	t_shell_env	*env;
	char		*key;
	char		*value;
	int			i;

	env = env_create(gc);
	if (!env)
		return (NULL);
	i = 0;
	while (envp && envp[i])
	{
		if (parse_env_string(envp[i], &key, &value) == 0)
		{
			env_set_var(gc, env, key, value);
			free(key);
			free(value);
		}
		i++;
	}
	env_update_pwd(gc, env);
	return (env);
}

void	env_update_pwd(t_gc *gc, t_shell_env *env)
{
	char	*pwd;

	if (!env)
		return ;
	pwd = getcwd(NULL, 0);
	if (!pwd)
		return ;
	env_set_var(gc, env, "PWD", pwd);
	if (env->cwd)
		env->cwd = gc_malloc(gc, ft_strlen(pwd) + 1);
	if (env->cwd)
		ft_strlcpy(env->cwd, pwd, ft_strlen(pwd) + 1);
	free(pwd);
}

void	env_set_exit_code(t_shell_env *env, int exit_code)
{
	if (!env)
		return ;
	env->last_exit_code = exit_code;
}
