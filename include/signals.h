/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexanfe <alexanfe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/19 17:18:04 by alexanfe          #+#    #+#             */
/*   Updated: 2025/09/19 17:18:05 by alexanfe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SIGNALS_H
# define SIGNALS_H

# include <signal.h>

extern int	g_signal_received;

void	setup_shell_signals(void);
void	setup_command_signals(void);
void	handle_sigint(int sig);
void	reset_signal_received(void);
int		get_signal_received(void);
int		process_child_status(int status);
#endif
