/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_handler.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpaulo-c <rpaulo-c@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 20:11:36 by rpaulo-c          #+#    #+#             */
/*   Updated: 2025/09/19 05:44:54 by rpaulo-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "signals.h"

void	setup_signals(void)
{
	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, SIG_IGN);
}

void	handle_sigint(int sig)
{
	*get_signal_received() = sig;
	if (get_shell_state()->in_command)
	{
		return ;
	}
	write(STDOUT_FILENO, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	set_command_mode(int in_command)
{
	t_shell_state	*shell;

	shell = get_shell_state();
	shell->in_command = in_command;
	if (in_command)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
	}
	else
	{
		setup_signals();
	}
}