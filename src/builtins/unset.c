/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elvictor <elvictor@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 14:28:29 by elvictor          #+#    #+#             */
/*   Updated: 2025/09/09 16:52:02 by elvictor         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	key_is_valid(const char *key)
{
	size_t	i;

	i = 1;
	if (!key || (!ft_isalpha(key[0]) && key[0] != '_'))
		return (0);
	while (key[i])
	{
		if (!ft_isalnum(key[i]) && key[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

static void	delete_env_var(const char *key)
{
	char	**env;
	size_t	len;
	int		i;

	env = get_envp(NULL);
	if (!env)
		return ;
	len = ft_strlen(key);
	i = 0;
	while (env[i] && (ft_strncmp(env[i], key, len) != 0 || env[i][len] != '='))
		i++;
	if (env[i])
	{
		free(env[i]);
		while (env[i + 1])
		{
			env[i] = env[i + 1];
			i++;
		}
		env[i] = NULL;
	}
}

int	exec_unset(char **args)
{
	int	i;

	i = 1;
	while (args[i])
	{
		if (key_is_valid(args[i]))
			delete_env_var(args[i]);
		i++;
	}
	return (0);
}