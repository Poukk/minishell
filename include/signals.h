/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpaulo-c <rpaulo-c@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 20:01:22 by rpaulo-c          #+#    #+#             */
/*   Updated: 2025/09/15 20:06:43 by rpaulo-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SIGNALS_H
# define SIGNALS_H

# include <signal.h>
# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/wait.h>
# include <readline/readline.h>
# include <readline/history.h>

typedef struct s_shell_state
{
	int		exit_status;
	int		in_command;
}	t_shell_state;

int				*get_signal_received(void);

t_shell_state	*get_shell_state(void);

void			handle_sigint(int sig);

void			setup_signals(void);

void			set_command_mode(int in_command);

int				process_child_status(int status);

#endif