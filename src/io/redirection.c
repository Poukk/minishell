/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexanfe <alexanfe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 00:00:00 by alexanfe          #+#    #+#             */
/*   Updated: 2025/09/19 17:48:44 by alexanfe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <fcntl.h>

static int	write_content_to_pipe(int write_fd, char *content,
		size_t content_len)
{
	ssize_t	bytes_written;

	if (content_len == 0)
		return (0);
	bytes_written = write(write_fd, content, content_len);
	if (bytes_written == -1)
	{
		ft_dprintf(2, "minishell: heredoc write failed\n");
		return (-1);
	}
	return (0);
}

static int	setup_pipe_stdin(int read_fd)
{
	if (dup2(read_fd, STDIN_FILENO) == -1)
	{
		ft_dprintf(2, "minishell: heredoc redirection failed\n");
		return (-1);
	}
	return (0);
}

static size_t	get_heredoc_content_len(t_redirection *redir)
{
	if (!redir->heredoc_content)
		return (0);
	return (ft_strlen(redir->heredoc_content));
}

static int	create_and_write_pipe(t_redirection *redir, int *pipefd)
{
	size_t	content_len;

	if (pipe(pipefd) == -1)
	{
		ft_dprintf(2, "minishell: pipe failed\n");
		return (-1);
	}
	content_len = get_heredoc_content_len(redir);
	if (write_content_to_pipe(pipefd[1], redir->heredoc_content,
			content_len) == -1)
	{
		close(pipefd[0]);
		close(pipefd[1]);
		return (-1);
	}
	return (0);
}

int	setup_heredoc_redirection(t_redirection *redir)
{
	int		pipefd[2];

	if (!redir)
		return (0);
	if (create_and_write_pipe(redir, pipefd) == -1)
		return (-1);
	close(pipefd[1]);
	if (setup_pipe_stdin(pipefd[0]) == -1)
	{
		close(pipefd[0]);
		return (-1);
	}
	close(pipefd[0]);
	return (0);
}
