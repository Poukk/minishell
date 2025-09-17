/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexanfe <alexanfe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 02:03:46 by alexanfe          #+#    #+#             */
/*   Updated: 2025/09/17 02:04:35 by alexanfe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_file_open_error(char *filename)
{
	if (access(filename, F_OK) == -1)
		ft_printf("minishell: %s: No such file or directory\n", filename);
	else
		ft_printf("minishell: %s: Permission denied\n", filename);
	return (-1);
}
