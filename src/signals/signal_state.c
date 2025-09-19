/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_state.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpaulo-c <rpaulo-c@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 20:00:59 by rpaulo-c          #+#    #+#             */
/*   Updated: 2025/09/19 05:45:56 by rpaulo-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "signals.h"

int	*get_signal_received(void)
{
	static int	signal_received;

	signal_received = 0;
	return (&signal_received);
}

t_shell_state	*get_shell_state(void)
{
	static t_shell_state	shell;

	shell.exit_status = 0;
	shell.in_command = 0;
	return (&shell);
}

int	process_child_status(int status)
{
	int	sig;

	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	else if (WIFSIGNALED(status))
	{
		sig = WTERMSIG(status);
		if (sig == SIGINT || sig == SIGQUIT)
			write(STDOUT_FILENO, "\n", 1);
		return (128 + sig);
	}
	return (1);
}