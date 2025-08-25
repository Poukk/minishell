/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elvictor <elvictor@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 23:29:54 by alexanfe          #+#    #+#             */
/*   Updated: 2025/08/25 15:39:45 by elvictor         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdlib.h>
# include <stdio.h>
# include <unistd.h>
# include <string.h>
# include "libft.h"

typedef struct s_gc
{
	t_list	*allocated_ptrs;
}	t_gc;

int		sum(int a, int b);
void	*gc_malloc(t_gc *gc, size_t size);
void	gc_free_all(t_gc *gc);
void	gc_init(t_gc *gc);

//BUILTINS
t_bool	is_builtin(const char *cmd);
int		exec_builtin(char **args, int last_status);
int		exec_pwd(char **args);

t_token	*lexer_tokenize(t_gc *gc, const char *input);
t_token	*token_create(t_gc *gc, t_token_type type, const char *value);
void	token_add_back(t_token **head, t_token *new_token);
void	token_print_list(t_token *tokens);

t_token	*handle_metachar(t_gc *gc, const char **input);
char	*extract_quoted_string(t_gc *gc, const char **input, char quote);
char	*extract_word(t_gc *gc, const char **input);
t_token	*handle_word_or_quote(t_gc *gc, const char **input);
#endif
