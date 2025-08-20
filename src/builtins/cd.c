/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elvictor <elvictor@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 17:36:57 by elvictor          #+#    #+#             */
/*   Updated: 2025/08/20 19:16:52 by elvictor         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	exec_cd(char **args)
{
	const char	*newpwd;
	char		*oldpwd;
	int			argc;

	argc = 0;
	while (args[argc])
		argc++;
	if (argc > 2)
		return (print_cd_erro("cd:so much arguments\n"));
	if (!args[1])
	{
		oldpwd = getenv("HOME");
		if (!oldpwd)
			return (printf_cd_erro("cd: failed to get current directory\n"));
	}


		/*
	oldpwd = getcwd(NULL, 0);
	if (!oldpwd)
		return (handle_error("cd: failed to get current directory"));
	if (chdir(args[1]) != 0)
	{
		perror("cd");
		free(oldpwd);
		return (FAILURE);
	}
	newpwd = getcwd(NULL, 0);
	if (newpwd)
	{
		set_env("OLDPWD", oldpwd);
		set_env("PWD", newpwd);
		free(newpwd);
	}
	free(oldpwd);
	return (SUCCESS);
}
*/
}
