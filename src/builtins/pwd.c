/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elvictor <elvictor@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 14:28:29 by elvictor          #+#    #+#             */
/*   Updated: 2025/08/20 15:10:17 by elvictor         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	exec_pwd(char **args)
{
	char	*cwd;

	(void)args;
	cwd = getcwd(NULL, 0);
	if (!cwd)
	{
		perror("pwd");
		return (FAILURE);
	}
	printf("%s\n", cwd);
	free(cwd);
	return (SUCCESS);
}
/*
int	main(int argc,char **argv)
{
	(void)argc;
	return (exec_pwd(argv));
}
*/