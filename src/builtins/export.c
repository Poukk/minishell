/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elvictor <elvictor@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 17:20:15 by elvictor          #+#    #+#             */
/*   Updated: 2025/09/11 18:07:01 by elvictor         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	exec_export(char **args)
{
	int		i;
	int		status;
	char	**env;

	env = get_envp(NULL);
	if (!args[1])
		return (print_export_list(env));
	status = 0;
	i = 1;
	while (args[i])
	{
		process_export_arg(args[i], &env, &status);
		i++;
	}
	return (status);
}
