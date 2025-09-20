/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_redir_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexanfe <alexanfe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 12:00:00 by alexanfe          #+#    #+#             */
/*   Updated: 2025/01/20 12:00:00 by alexanfe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	redirection_entry_add_ordered(t_redirection_entry **head,
	t_redirection_entry *new_entry)
{
	t_redirection_entry	*current;
	t_redirection_entry	*prev;

	if (!head || !new_entry)
		return ;
	if (!*head || (*head)->position > new_entry->position)
	{
		new_entry->next = *head;
		*head = new_entry;
		return ;
	}
	prev = NULL;
	current = *head;
	while (current && current->position <= new_entry->position)
	{
		prev = current;
		current = current->next;
	}
	new_entry->next = current;
	prev->next = new_entry;
}

t_redirection_entry	*redirection_entry_get_by_position(
	t_redirection_entry *head, int position)
{
	t_redirection_entry	*current;

	current = head;
	while (current)
	{
		if (current->position == position)
			return (current);
		current = current->next;
	}
	return (NULL);
}
