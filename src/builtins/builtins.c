/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexanfe <alexanfe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/19 01:39:35 by alexanfe          #+#    #+#             */
/*   Updated: 2025/09/19 01:39:36 by alexanfe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_builtin_type	find_builtin_type(const char *cmd_name)
{
	if (ft_strncmp(cmd_name, "echo", 4) == 0 && !cmd_name[4])
		return (BUILTIN_ECHO);
	if (ft_strncmp(cmd_name, "cd", 2) == 0 && !cmd_name[2])
		return (BUILTIN_CD);
	if (ft_strncmp(cmd_name, "pwd", 3) == 0 && !cmd_name[3])
		return (BUILTIN_PWD);
	if (ft_strncmp(cmd_name, "env", 3) == 0 && !cmd_name[3])
		return (BUILTIN_ENV);
	if (ft_strncmp(cmd_name, "export", 6) == 0 && !cmd_name[6])
		return (BUILTIN_EXPORT);
	if (ft_strncmp(cmd_name, "unset", 5) == 0 && !cmd_name[5])
		return (BUILTIN_UNSET);
	if (ft_strncmp(cmd_name, "exit", 4) == 0 && !cmd_name[4])
		return (BUILTIN_EXIT);
	return (BUILTIN_NONE);
}

t_builtin_type	is_builtin_command(const char *cmd_name)
{
	if (!cmd_name || !*cmd_name)
		return (BUILTIN_NONE);
	return (find_builtin_type(cmd_name));
}

static int	dispatch_builtin(t_builtin_type type, char **args,
		t_shell_context *ctx)
{
	if (type == BUILTIN_ECHO)
		return (builtin_echo(args));
	if (type == BUILTIN_CD)
		return (builtin_cd(args, ctx));
	if (type == BUILTIN_PWD)
		return (builtin_pwd());
	if (type == BUILTIN_ENV)
		return (builtin_env(ctx));
	if (type == BUILTIN_EXPORT)
		return (builtin_export(args, ctx));
	if (type == BUILTIN_UNSET)
		return (builtin_unset(args, ctx));
	if (type == BUILTIN_EXIT)
		return (builtin_exit(args, ctx));
	return (1);
}

int	execute_builtin(t_builtin_type type, char **args, t_shell_context *ctx)
{
	int	result;

	if (!args || type == BUILTIN_NONE)
		return (1);
	if (!ctx || !ctx->env)
		return (1);
	if (type != BUILTIN_ECHO && type != BUILTIN_PWD && !ctx->env)
		return (1);
	result = dispatch_builtin(type, args, ctx);
	if (ctx->env)
		env_set_exit_code(ctx->env, result);
	return (result);
}
