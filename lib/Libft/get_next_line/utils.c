/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexanfe <alexanfe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 21:04:23 by alexanfe          #+#    #+#             */
/*   Updated: 2025/03/27 21:04:25 by alexanfe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

t_buffer	*init_clean_buffer(int fd)
{
	t_buffer	*buffer;
	int			i;

	buffer = (t_buffer *)malloc(sizeof(t_buffer));
	if (!buffer)
		return (NULL);
	buffer->fd = fd;
	buffer->position = 0;
	i = 0;
	while (i < BUFFER_SIZE)
		buffer->buffer[i++] = '\0';
	buffer->string = NULL;
	buffer->length = 0;
	buffer->total_read = 0;
	buffer->next = NULL;
	return (buffer);
}

t_buffer	*get_buffer(t_buffer **head, int fd)
{
	t_buffer	*current;

	if (!*head)
	{
		*head = init_clean_buffer(fd);
		return (*head);
	}
	current = *head;
	while (current)
	{
		if (current->fd == fd)
			return (current);
		if (!current->next)
		{
			current->next = init_clean_buffer(fd);
			return (current->next);
		}
		current = current->next;
	}
	return (NULL);
}

void	remove_buffer(t_buffer **head, int fd)
{
	t_buffer	*current;
	t_buffer	*prev;

	if (!*head)
		return ;
	if ((*head)->fd == fd)
	{
		current = *head;
		*head = (*head)->next;
		free(current);
		return ;
	}
	prev = *head;
	current = (*head)->next;
	while (current)
	{
		if (current->fd == fd)
		{
			prev->next = current->next;
			free(current);
			return ;
		}
		prev = current;
		current = current->next;
	}
}

void	clear_string(t_string *str)
{
	t_string	*temp;

	while (str)
	{
		temp = str->next;
		free(str);
		str = temp;
	}
	temp = NULL;
}

void	build_string(t_string **string_head, char character)
{
	t_string	*new_character;
	t_string	*current;

	new_character = (t_string *)malloc(sizeof(t_string));
	if (!new_character)
	{
		clear_string(*string_head);
		*string_head = NULL;
		return ;
	}
	new_character->character = character;
	new_character->next = NULL;
	if (!*string_head)
	{
		*string_head = new_character;
		return ;
	}
	current = *string_head;
	while (current->next)
		current = current->next;
	current->next = new_character;
}
