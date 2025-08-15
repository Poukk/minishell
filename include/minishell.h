/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexanfe <alexanfe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 23:29:54 by alexanfe          #+#    #+#             */
/*   Updated: 2025/06/16 23:29:59 by alexanfe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft.h"

typedef struct s_gc
{
	t_list	*allocated_ptrs;
}	t_gc;

int		sum(int a, int b);
void	*gc_malloc(t_gc *gc, size_t size);
void	gc_free_all(t_gc *gc);
void	gc_init(t_gc *gc);

#endif
