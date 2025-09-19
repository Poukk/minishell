/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gc.h                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexanfe <alexanfe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/19 17:13:09 by alexanfe          #+#    #+#             */
/*   Updated: 2025/09/19 17:13:11 by alexanfe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GC_H
# define GC_H

# include "libft.h"
# include <stdlib.h>

typedef struct s_gc
{
	t_list	*allocated_ptrs;
}	t_gc;

void	gc_init(t_gc *gc);
void	*gc_malloc(t_gc *gc, size_t size);
void	gc_free_all(t_gc *gc);
char	*gc_readline(t_gc *gc, const char *prompt);
char	*gc_strdup(t_gc *gc, const char *s);

#endif
