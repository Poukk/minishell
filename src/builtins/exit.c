/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elvictor <elvictor@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 14:28:29 by elvictor          #+#    #+#             */
/*   Updated: 2025/09/19 01:41:32 by alexanfe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	parse_exit_code(char *arg)
{
	int		result;
	char	*endptr;

	if (!arg)
		return (0);
	result = ft_strtol(arg, &endptr, 10);
	if (*endptr != '\0')
	{
		ft_printf("exit: %s: numeric argument required\n", arg);
		exit(2);
	}
	return (result);
}

static int	count_args(char **args)
{
	int	count;

	count = 0;
	while (args[count])
		count++;
	return (count);
}

int	builtin_exit(char **args, t_shell_context *ctx)
{
	int	exit_code;

	(void)ctx;
	ft_printf("exit\n");
	if (count_args(args) > 2)
	{
		ft_printf("exit: too many arguments\n");
		return (1);
	}
	exit_code = parse_exit_code(args[1]);
	exit(exit_code);
}
