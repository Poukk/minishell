/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elvictor <elvictor@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 14:28:29 by elvictor          #+#    #+#             */
/*   Updated: 2025/09/19 01:42:27 by alexanfe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	builtin_unset(char **args, t_shell_context *ctx)
{
	int	i;

	if (!ctx || !ctx->env)
		return (1);
	if (!args[1])
		return (0);
	i = 1;
	while (args[i])
	{
		env_unset_var(ctx->env, args[i]);
		i++;
	}
	return (0);
}
