/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexanfe <alexanfe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 02:03:46 by alexanfe          #+#    #+#             */
/*   Updated: 2025/09/19 17:01:20 by alexanfe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_file_open_error(char *filename)
{
	if (access(filename, F_OK) == -1)
		ft_dprintf(2, "minishell: %s: No such file or directory\n", filename);
	else
		ft_dprintf(2, "minishell: %s: Permission denied\n", filename);
	return (-1);
}

void	print_error(const char *command, const char *arg, const char *message)
{
	ft_dprintf(2, "minishell: ");
	if (command)
		ft_dprintf(2, "%s: ", command);
	if (arg)
		ft_dprintf(2, "%s: ", arg);
	ft_dprintf(2, "%s\n", message);
}

void	print_command_error(const char *command, const char *message)
{
	print_error(command, NULL, message);
}

int	return_error_code(int error_code, const char *command, const char *arg,
		const char *message)
{
	if (message)
		print_error(command, arg, message);
	return (error_code);
}
