/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexanfe <alexanfe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 23:29:54 by alexanfe          #+#    #+#             */
/*   Updated: 2025/08/22 17:27:22 by alexanfe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft.h"
# include <stdlib.h>

typedef struct s_gc
{
	t_list	*allocated_ptrs;
}	t_gc;

typedef enum e_token_type
{
	TOKEN_WORD,
	TOKEN_PIPE,
	TOKEN_REDIR_IN,
	TOKEN_REDIR_OUT,
	TOKEN_REDIR_APPEND,
	TOKEN_HEREDOC,
	TOKEN_EOF
}	t_token_type;

typedef struct s_token
{
	t_token_type	type;
	char			*value;
	struct s_token	*next;
}	t_token;

int		sum(int a, int b);
void	*gc_malloc(t_gc *gc, size_t size);
void	gc_free_all(t_gc *gc);
void	gc_init(t_gc *gc);

t_token	*lexer_tokenize(t_gc *gc, const char *input);
t_token	*token_create(t_gc *gc, t_token_type type, const char *value);
void	token_add_back(t_token **head, t_token *new_token);
void	token_print_list(t_token *tokens);

t_token	*handle_metachar(t_gc *gc, const char **input);
char	*extract_quoted_string(t_gc *gc, const char **input, char quote);
char	*extract_word(t_gc *gc, const char **input);
t_token	*handle_word_or_quote(t_gc *gc, const char **input);
#endif
