/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_ordered.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexanfe <alexanfe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 12:00:00 by alexanfe          #+#    #+#             */
/*   Updated: 2025/01/20 12:00:00 by alexanfe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <fcntl.h>

static int	process_input_redirection_entry(t_redirection_entry *entry)
{
	int				fd;
	t_redirection	redir;

	if (entry->type == TOKEN_HEREDOC)
	{
		redir.type = TOKEN_HEREDOC;
		redir.filename = entry->filename;
		redir.heredoc_content = entry->heredoc_content;
		redir.next = NULL;
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
