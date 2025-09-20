/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_state.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpaulo-c <rpaulo-c@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 20:00:59 by rpaulo-c          #+#    #+#             */
/*   Updated: 2025/09/19 14:29:53 by alexanfe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t	g_signal_received = 0;

sig_atomic_t	get_signal_received(void)
{
	return (g_signal_received);
}

void	reset_signal_received(void)
{
	g_signal_received = 0;
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
