/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elvictor <elvictor@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 16:48:47 by elvictor          #+#    #+#             */
/*   Updated: 2025/09/09 16:40:33 by elvictor         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	update_pwd(void)
{
	char	*cwd;

	cwd = getcwd(NULL, 0);
	if (!cwd)
		return ;
	free(cwd);
}

static int	print_cd_err(char *msg)
{
	ft_printf_fd(2, "%s", msg);
	return (1);
}

int	exec_cd(char **args)
{
	const char	*path;
	char		*home;
	int			argc;

	argc = 0;
	while (args[argc])
		argc++;
	if (argc > 2)
		return (print_cd_err("cd: too many arguments\n"));
	if (!args[1] || !ft_strcmp(args[1], "~"))
	{
		home = getenv("HOME");
		if (!home)
			return (print_cd_err("cd: HOME not set\n"));
		path = home;
	}
	else
		path = args[1];
	if (chdir(path) != 0)
	{
		ft_printf_fd(2, "cd: %s: No such file or directory\n", path);
		return (1);
	}
	update_pwd();
	return (0);
}
