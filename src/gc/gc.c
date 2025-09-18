/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gc.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexanfe <alexanfe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/15 00:00:00 by alexanfe          #+#    #+#             */
/*   Updated: 2025/08/15 00:00:00 by alexanfe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	gc_init(t_gc *gc)
{
	gc->allocated_ptrs = NULL;
}

void	*gc_malloc(t_gc *gc, size_t size)
{
	void	*ptr;
	t_list	*node;

	ptr = malloc(size);
	if (!ptr)
		return (NULL);
	node = ft_lstnew(ptr);
	if (!node)
	{
		free(ptr);
		return (NULL);
	}
	ft_lstadd_front(&gc->allocated_ptrs, node);
	return (ptr);
}

void	gc_free_all(t_gc *gc)
{
	ft_lstclear(&gc->allocated_ptrs, free);
	gc->allocated_ptrs = NULL;
}

char	*gc_readline(t_gc *gc, const char *prompt)
{
	char	*line;
	char	*gc_line;

	line = readline(prompt);
	if (!line)
		return (NULL);
	gc_line = gc_malloc(gc, ft_strlen(line) + 1);
	if (!gc_line)
	{
		free(line);
		return (NULL);
	}
	ft_strlcpy(gc_line, line, ft_strlen(line) + 1);
	free(line);
	return (gc_line);
}
