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

static int	process_input_redirection_entry(t_redirection_entry *entry)
{
	int	fd;

	if (entry->type == TOKEN_HEREDOC)
	{
		t_redirection redir = {TOKEN_HEREDOC, entry->filename, entry->heredoc_content, NULL};
		return (setup_heredoc_redirection(&redir));
	}
	fd = open(entry->filename, O_RDONLY);
	if (fd == -1)
		return (handle_file_open_error(entry->filename));
	if (dup2(fd, STDIN_FILENO) == -1)
	{
		close(fd);
		ft_dprintf(2, "minishell: redirection failed\n");
		return (-1);
	}
	close(fd);
	return (0);
}

static int	process_output_redirection_entry(t_redirection_entry *entry)
{
	int	fd;
	int	flags;

	if (entry->type == TOKEN_REDIR_APPEND)
		flags = O_WRONLY | O_CREAT | O_APPEND;
	else
		flags = O_WRONLY | O_CREAT | O_TRUNC;
	
	fd = open(entry->filename, flags, 0644);
	if (fd == -1)
	{
		ft_dprintf(2, "minishell: %s: Permission denied\n", entry->filename);
		return (-1);
	}
	if (dup2(fd, STDOUT_FILENO) == -1)
	{
		close(fd);
		ft_dprintf(2, "minishell: redirection failed\n");
		return (-1);
	}
	close(fd);
	return (0);
}

static int	is_input_redirection(int type)
{
	return (type == TOKEN_REDIR_IN || type == TOKEN_HEREDOC);
}

static int	is_output_redirection(int type)
{
	return (type == TOKEN_REDIR_OUT || type == TOKEN_REDIR_APPEND);
}

int	setup_redirections_ordered(t_redirection_entry *redirections)
{
	t_redirection_entry	*current;

	current = redirections;
	while (current)
	{
		if (is_input_redirection(current->type))
		{
			if (process_input_redirection_entry(current) == -1)
				return (-1);
		}
		else if (is_output_redirection(current->type))
		{
			if (process_output_redirection_entry(current) == -1)
				return (-1);
		}
		current = current->next;
	}
	return (0);
}
