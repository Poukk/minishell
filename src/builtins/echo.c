/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elvictor <elvictor@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 19:32:25 by elvictor          #+#    #+#             */
/*   Updated: 2025/08/09 21:02:16 by elvictor         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int has_parametres_n(char **args)
{
	int	i;
	int	j;

	i = 1;
	while (args[i] && args[i][0] == '-' && args[i][1] == 'n')
	{
		j = 2;
		while (args[i][j] == 'n')
			j++;
		if (args[i][j] == '\0')
			return (1);
		break ;
	}
	return (0);
}

int	exec_echo(char **args)
{
	int	i;
	int	newline;

	newline = 1;
	i = 1;
	if (has_parametres_n(args))
	{
		newline = 0;
		i++;
	}
	while (args[i])
	{
		printf("%s", args[i]);
		if (args[i + 1])
			printf(" ");
		i++;
	}
	if (newline)
		printf("\n");
	return (SUCCESS);
}

int	main(int args, char **argv)
{
	argv++;
	exec_echo(argv);
}
