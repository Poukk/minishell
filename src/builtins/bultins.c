/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bultins.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elvictor <elvictor@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 16:11:47 by elvictor          #+#    #+#             */
/*   Updated: 2025/08/13 15:53:44 by elvictor         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_bool	is_builtin(const char *cmd)
{
	if (!ft_strcmp(cmd, "cd") || !ft_strcmp(cmd, "echo")
		|| !ft_strcmp(cmd, "pwd") || !ft_strcmp(cmd, "export")
		|| !ft_strcmp(cmd, "unset") || !ft_strcmp(cmd, "env")
		|| !ft_strcmp(cmd, "exit"))
		return (TRUE);
	return (FALSE);
}

int	exec_builtin(char **args, int last_status)
{
	if (!args || !args[0])
		return (1);
	if (!ft_strcmp(args[0], "cd"))
		return (exec_cd(args));
	if (!ft_strcmp(args[0], "echo"))
		return (exec_echo(args));
	if (!ft_strcmp(args[0], "pwd"))
		return (exec_pwd(args));
	if (!ft_strcmp(args[0], "export"))
		return (exec_export(args));
	if (!ft_strcmp(args[0], "unset"))
		return (exec_unset(args));
	if (!ft_strcmp(args[0], "env"))
		return (exec_env(args));
	if (!ft_strcmp(args[0], "exit"))
		return (exec_exit(args, last_status));
	return (handle_error("error executing builtin")); //TODO
}
