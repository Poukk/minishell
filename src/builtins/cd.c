/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elvictor <elvictor@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 16:48:47 by elvictor          #+#    #+#             */
/*   Updated: 2025/09/19 01:40:10 by alexanfe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*get_target_directory(char **args, t_shell_context *ctx)
{
	if (!args[1])
		return (env_get_value(ctx->env, "HOME"));
	return (args[1]);
}

static int	count_args(char **args)
{
	int	count;

	count = 0;
	while (args[count])
		count++;
	return (count);
}

int	builtin_cd(char **args, t_shell_context *ctx)
{
	char	*target_dir;

	if (!ctx || !ctx->env)
		return (1);
	if (count_args(args) > 2)
	{
		ft_printf("cd: too many arguments\n");
		return (1);
	}
	target_dir = get_target_directory(args, ctx);
	if (!target_dir)
	{
		ft_printf("cd: HOME not set\n");
		return (1);
	}
	if (chdir(target_dir) != 0)
	{
		ft_printf("cd: %s: No such file or directory\n", target_dir);
		return (1);
	}
	env_update_pwd(ctx->gc, ctx->env);
	return (0);
}
