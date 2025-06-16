/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexanfe <alexanfe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 21:04:04 by alexanfe          #+#    #+#             */
/*   Updated: 2025/03/27 21:04:06 by alexanfe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# include <stdlib.h>
# include <unistd.h>

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 32
# endif

typedef struct s_string
{
	char			character;
	struct s_string	*next;
}	t_string;

typedef struct s_buffer
{
	int				fd;
	int				position;
	int				total_read;
	int				length;
	char			buffer[BUFFER_SIZE];
	t_string		*string;
	struct s_buffer	*next;
}	t_buffer;

char		*get_next_line(int fd);
t_buffer	*init_clean_buffer(int fd);
void		clear_string(t_string *str);
void		build_string(t_string **string_head, char character);
char		*alloc_line(t_buffer *buffer);
char		*get_current_line(t_buffer *buffer);
t_buffer	*get_buffer(t_buffer **head, int fd);
void		remove_buffer(t_buffer **head, int fd);

#endif
