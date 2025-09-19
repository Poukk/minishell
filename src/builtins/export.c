/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elvictor <elvictor@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 17:20:15 by elvictor          #+#    #+#             */
/*   Updated: 2025/09/19 01:41:51 by alexanfe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	process_export_with_value(char *arg, char *equals_pos,
		t_shell_context *ctx)
{
	char	*key;
	char	*value;
	size_t	key_len;

	key_len = equals_pos - arg;
	key = gc_malloc(ctx->gc, key_len + 1);
	if (!key)
		return (1);
	ft_strlcpy(key, arg, key_len + 1);
	if (!env_is_valid_name(key))
	{
		ft_dprintf(2, "export: '%s': not a valid identifier\n", arg);
		return (1);
	}
	value = equals_pos + 1;
	if (env_set_var(ctx->gc, ctx->env, key, value) == -1)
		return (1);
	return (0);
}

static int	process_export_no_value(char *arg, t_shell_context *ctx)
{
	if (!env_is_valid_name(arg))
	{
		ft_dprintf(2, "export: '%s': not a valid identifier\n", arg);
		return (1);
	}
	if (env_set_var(ctx->gc, ctx->env, arg, "") == -1)
		return (1);
	return (0);
}

static int	process_export_arg(char *arg, t_shell_context *ctx)
{
	char	*equals_pos;

	equals_pos = ft_strchr(arg, '=');
	if (equals_pos)
		return (process_export_with_value(arg, equals_pos, ctx));
	else
		return (process_export_no_value(arg, ctx));
}

int	builtin_export(char **args, t_shell_context *ctx)
{
	int	i;
	int	result;

	if (!ctx || !ctx->env)
		return (1);
	if (!args[1])
	{
		env_print_all(ctx->env);
		return (0);
	}
	result = 0;
	i = 1;
	while (args[i])
	{
		if (process_export_arg(args[i], ctx) != 0)
			result = 1;
		i++;
	}
	return (result);
}
