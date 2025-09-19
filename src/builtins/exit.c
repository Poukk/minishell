/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elvictor <elvictor@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 14:28:29 by elvictor          #+#    #+#             */
/*   Updated: 2025/09/09 16:52:02 by elvictor         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	exec_exit(char **args, int last_status)
{
    long long	nb;

	ft_printf_fd(STDOUT_FILENO, "exit\n");
	if (!args[1])
		clean_and_exit(last_status);
	if (!ft_strtoll_check(args[1], &nb))
	{
		ft_printf_fd(STDERR_FILENO,
			"exit: %s: numeric argument required\n", args[1]);
		clean_and_exit(2);
	}
	if (args[2])
	{
		ft_printf_fd(STDERR_FILENO, "exit: too many arguments\n");
		return (FAILURE);
    }
    clean_and_exit((unsigned char)nb);
	return (SUCCESS);
}