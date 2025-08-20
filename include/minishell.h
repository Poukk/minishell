/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elvictor <elvictor@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 23:29:54 by alexanfe          #+#    #+#             */
/*   Updated: 2025/08/20 15:14:36 by elvictor         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdlib.h>
# include <stdio.h>
# include <unistd.h>
# include <string.h>
# include "libft.h"

# define SUCCESS 0
# define FAILURE 1

typedef struct s_gc
{
	t_list	*allocated_ptrs;
}	t_gc;

typedef enum e_bool
{
	FALSE,
	TRUE
}	t_bool;

int		sum(int a, int b);
void	*gc_malloc(t_gc *gc, size_t size);
void	gc_free_all(t_gc *gc);
void	gc_init(t_gc *gc);

//BUILTINS
t_bool	is_builtin(const char *cmd);
int		exec_builtin(char **args, int last_status);
int		exec_pwd(char **args);

#endif
