/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_redir.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexanfe <alexanfe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 01:22:27 by alexanfe          #+#    #+#             */
/*   Updated: 2025/09/15 01:22:28 by alexanfe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_redirection	*redirection_create(t_gc *gc, int type, char *filename)
{
	t_redirection	*redir;

	redir = gc_malloc(gc, sizeof(t_redirection));
	if (!redir)
		return (NULL);
	redir->type = type;
	redir->filename = gc_malloc(gc, ft_strlen(filename) + 1);
	if (!redir->filename)
		return (NULL);
	ft_strlcpy(redir->filename, filename, ft_strlen(filename) + 1);
	redir->heredoc_content = NULL;
	redir->next = NULL;
	return (redir);
}

t_redirection	*heredoc_redirection_create(t_gc *gc, char *delimiter,
	char *content)
{
	t_redirection	*redir;

	redir = gc_malloc(gc, sizeof(t_redirection));
	if (!redir)
		return (NULL);
	redir->type = TOKEN_HEREDOC;
	redir->filename = gc_malloc(gc, ft_strlen(delimiter) + 1);
	if (!redir->filename)
		return (NULL);
	ft_strlcpy(redir->filename, delimiter, ft_strlen(delimiter) + 1);
	if (content)
	{
		redir->heredoc_content = gc_malloc(gc, ft_strlen(content) + 1);
		if (!redir->heredoc_content)
			return (NULL);
		ft_strlcpy(redir->heredoc_content, content, ft_strlen(content) + 1);
	}
	else
		redir->heredoc_content = NULL;
	redir->next = NULL;
	return (redir);
}

void	redirection_add_back(t_redirection **head, t_redirection *new_redir)
{
	t_redirection	*current;

	if (!head || !new_redir)
		return ;
	if (!*head)
	{
		*head = new_redir;
		return ;
	}
	current = *head;
	while (current->next)
		current = current->next;
	current->next = new_redir;
}

t_redirection_entry	*redirection_entry_create(t_gc *gc, int type,
	char *filename, int position)
{
	t_redirection_entry	*entry;

	entry = gc_malloc(gc, sizeof(t_redirection_entry));
	if (!entry)
		return (NULL);
	entry->type = type;
	entry->filename = gc_malloc(gc, ft_strlen(filename) + 1);
	if (!entry->filename)
		return (NULL);
	ft_strlcpy(entry->filename, filename, ft_strlen(filename) + 1);
	entry->heredoc_content = NULL;
	entry->position = position;
	entry->next = NULL;
	return (entry);
}

t_redirection_entry	*heredoc_entry_create(t_gc *gc, char *delimiter,
		char *content, int position)
{
	t_redirection_entry	*entry;

	entry = gc_malloc(gc, sizeof(t_redirection_entry));
	if (!entry)
		return (NULL);
	entry->type = TOKEN_HEREDOC;
	entry->filename = gc_malloc(gc, ft_strlen(delimiter) + 1);
	if (!entry->filename)
		return (NULL);
	ft_strlcpy(entry->filename, delimiter, ft_strlen(delimiter) + 1);
	if (content)
	{
		entry->heredoc_content = gc_malloc(gc, ft_strlen(content) + 1);
		if (!entry->heredoc_content)
			return (NULL);
		ft_strlcpy(entry->heredoc_content, content, ft_strlen(content) + 1);
	}
	else
		entry->heredoc_content = NULL;
	entry->position = position;
	entry->next = NULL;
	return (entry);
}
