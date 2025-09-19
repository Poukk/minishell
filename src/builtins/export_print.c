/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_print.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elvictor <elvictor@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 18:08:23 by elvictor          #+#    #+#             */
/*   Updated: 2025/09/11 18:18:08 by elvictor         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**get_envp(char **envp)
{
	static char	**env;

	if ((!envp || !*envp) && !env)
		return (NULL);
	if (!envp || !*envp)
		return (env);
	if (env)
		ft_free_matrix(env);
	env = ft_strdup_split(envp);
	return (env);
}
