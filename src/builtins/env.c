/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elvictor <elvictor@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 19:32:25 by elvictor          #+#    #+#             */
/*   Updated: 2025/09/09 16:51:45 by elvictor         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int exec_env(char **args)
{
	int     i;
	char    **env;

	env = get_envp(NULL);
	if (args[1])
	{
		ft_printf_fd(2, "env: too many arguments\n");
		return (FAILURE);
	}
	i = 0;
	while (env[i])
	{
		if (ft_strchr(env[i], '=') && !(env[i][0] == '_' && env[i][1] == '='))
		    ft_printf_fd(1, "%s\n", env[i]);
		i++;
	}
	return (SUCCESS);
}

char	*get_var_from_env(const char *key, char **env)
{
	size_t	keylen;
	int		i;

	if (!key)
		return ("");
	keylen = ft_strlen(key);
	i = 0;
	while (env && env[i])
	{
		if (!ft_strncmp(env[i], key, keylen) && env[i][keylen] == '=')
			return (&env[i][keylen + 1]);
		i++;
	}
	return ("");
}