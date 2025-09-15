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
