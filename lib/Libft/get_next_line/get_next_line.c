/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexanfe <alexanfe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 21:04:47 by alexanfe          #+#    #+#             */
/*   Updated: 2025/03/27 21:04:49 by alexanfe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*get_next_line(int fd)
{
	static t_buffer	*buffer_list;
	t_buffer		*current;
	char			*line;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	current = get_buffer(&buffer_list, fd);
	if (!current)
		return (NULL);
	if (current->position >= current->total_read || current->position == 0)
	{
		current->total_read = read(fd, current->buffer, BUFFER_SIZE);
		if (current->total_read <= 0)
		{
			if (current->string)
				clear_string(current->string);
			remove_buffer(&buffer_list, fd);
			return (NULL);
		}
		current->position = 0;
	}
	line = get_current_line(current);
	if (!line)
		remove_buffer(&buffer_list, fd);
	return (line);
}

char	*get_current_line(t_buffer *buffer)
{
	buffer->length = 0;
	while (buffer->total_read > 0)
	{
		build_string(&buffer->string, buffer->buffer[buffer->position]);
		if (buffer->buffer[buffer->position] == '\n')
			break ;
		buffer->position++;
		buffer->length++;
		if (buffer->position >= buffer->total_read)
		{
			buffer->position = 0;
			buffer->total_read = read(buffer->fd, buffer->buffer, BUFFER_SIZE);
			if (buffer->total_read < 0)
			{
				clear_string(buffer->string);
				buffer->string = NULL;
				return (NULL);
			}
		}
	}
	buffer->position++;
	buffer->length++;
	return (alloc_line(buffer));
}

char	*alloc_line(t_buffer *buffer)
{
	t_string	*current;
	char		*line;
	int			i;

	line = (char *)malloc(buffer->length + 1);
	if (!line)
		return (NULL);
	i = 0;
	current = NULL;
	while (buffer->string)
	{
		current = buffer->string->next;
		line[i] = buffer->string->character;
		free(buffer->string);
		buffer->string = current;
		i++;
	}
	line[i] = '\0';
	return (line);
}
