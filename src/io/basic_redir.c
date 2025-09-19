/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   basic_redir.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexanfe <alexanfe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 01:36:44 by alexanfe          #+#    #+#             */
/*   Updated: 2025/09/19 17:48:22 by alexanfe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <fcntl.h>

static int	setup_file_input_redirection(char *filename)
{
	int	fd;

	fd = open(filename, O_RDONLY);
	if (fd == -1)
		return (handle_file_open_error(filename));
	if (dup2(fd, STDIN_FILENO) == -1)
	{
		close(fd);
		ft_dprintf(2, "minishell: redirection failed\n");
		return (-1);
	}
	close(fd);
	return (0);
}

int	setup_input_redirection(t_redirection *redir)
{
	if (!redir)
		return (0);
	if (redir->type == TOKEN_HEREDOC)
		return (setup_heredoc_redirection(redir));
	return (setup_file_input_redirection(redir->filename));
}

static int	get_output_flags(int redir_type)
{
	if (redir_type == TOKEN_REDIR_APPEND)
		return (O_WRONLY | O_CREAT | O_APPEND);
	return (O_WRONLY | O_CREAT | O_TRUNC);
}

int	setup_output_redirection(t_redirection *redir)
{
	int	fd;

	if (!redir)
		return (0);
	fd = open(redir->filename, get_output_flags(redir->type), 0644);
	if (fd == -1)
	{
		ft_dprintf(2, "minishell: %s: Permission denied\n", redir->filename);
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

int	create_all_output_files(t_redirection *output_redirs)
{
	t_redirection	*current;
	int				fd;

	current = output_redirs;
	while (current)
	{
		fd = open(current->filename, get_output_flags(current->type), 0644);
		if (fd == -1)
		{
			ft_dprintf(2, "minishell: %s: Permission denied\n",
				current->filename);
			return (-1);
		}
		close(fd);
		current = current->next;
	}
	return (0);
}
