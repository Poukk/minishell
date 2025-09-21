/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexanfe <alexanfe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/19 17:30:00 by alexanfe          #+#    #+#             */
/*   Updated: 2025/09/19 17:30:00 by alexanfe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERROR_H
# define ERROR_H

# define EXIT_SUCCESS		0
# define EXIT_GENERAL_ERROR	1
# define EXIT_MISUSE		2
# define EXIT_EXEC_FAILED	126
# define EXIT_CMD_NOT_FOUND	127

void	print_error(const char *command, const char *arg, const char *message);
void	print_command_error(const char *command, const char *message);
int		return_error_code(int error_code, const char *command, \
			const char *arg, const char *message);
int		handle_file_open_error(char *filename);

#endif
