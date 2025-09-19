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
	if (ft_strncmp(args[1], "-", 2) == 0)
		return (env_get_value(ctx->env, "OLDPWD"));
	return (args[1]);
}

static int	handle_cd_dash(t_shell_context *ctx)
{
	char	*oldpwd;

	oldpwd = env_get_value(ctx->env, "OLDPWD");
	if (!oldpwd)
	{
		ft_dprintf(2, "cd: OLDPWD not set\n");
		return (1);
	}
	ft_printf("%s\n", oldpwd);
	return (0);
}

static int	count_args(char **args)
{
	int	count;

	count = 0;
	while (args[count])
		count++;
	return (count);
}

static int	execute_directory_change(char **args, t_shell_context *ctx,
		char *current_pwd)
{
	char	*target_dir;

	target_dir = get_target_directory(args, ctx);
	if (!target_dir)
	{
		if (args[1] && ft_strncmp(args[1], "-", 2) == 0)
			ft_dprintf(2, "cd: OLDPWD not set\n");
		else
			ft_dprintf(2, "cd: HOME not set\n");
		return (1);
	}
	if (chdir(target_dir) != 0)
	{
		ft_dprintf(2, "cd: %s: No such file or directory\n", target_dir);
		return (1);
	}
	if (current_pwd)
		env_set_var(ctx->gc, ctx->env, "OLDPWD", current_pwd);
	env_update_pwd(ctx->gc, ctx->env);
	return (0);
}

int	builtin_cd(char **args, t_shell_context *ctx)
{
	char	*current_pwd;

	if (!ctx || !ctx->env)
		return (1);
	if (count_args(args) > 2)
	{
		ft_dprintf(2, "cd: too many arguments\n");
		return (2);
	}
	if (args[1] && ft_strncmp(args[1], "-", 2) == 0)
	{
		if (handle_cd_dash(ctx) != 0)
			return (1);
	}
	current_pwd = env_get_value(ctx->env, "PWD");
	return (execute_directory_change(args, ctx, current_pwd));
}
