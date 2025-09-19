/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexanfe <alexanfe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/19 17:13:30 by alexanfe          #+#    #+#             */
/*   Updated: 2025/09/19 17:13:31 by alexanfe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_H
# define LEXER_H

# include "gc.h"

typedef enum e_token_type
{
	TOKEN_WORD,
	TOKEN_PIPE,
	TOKEN_REDIR_IN,
	TOKEN_REDIR_OUT,
	TOKEN_REDIR_APPEND,
	TOKEN_HEREDOC,
	TOKEN_VARIABLE,
	TOKEN_EOF
}	t_token_type;

typedef struct s_token
{
	t_token_type	type;
	char			*value;
	char			quote_context;
	struct s_token	*next;
}	t_token;

typedef struct s_concat_result
{
	char	*content;
	char	quote_context;
}	t_concat_result;

t_token			*tokenize(t_gc *gc, const char *input);
t_token			*token_create(t_gc *gc, t_token_type type, const char *value);
t_token			*token_create_with_quote(t_gc *gc, t_token_type type,
					const char *value, char quote_context);
t_token			*handle_metachar(t_gc *gc, const char **input);
t_token			*handle_word_or_quote(t_gc *gc, const char **input);
char			*extract_word(t_gc *gc, const char **input);
char			*extract_concatenated_content(t_gc *gc, const char **input);
t_concat_result	extract_concatenated_content_with_context(t_gc *gc,
					const char **input);
char			*extract_quoted_string(t_gc *gc,
					const char **input, char quote);
char			*concatenate_strings(t_gc *gc, const char *str1,
					const char *str2);
char			*handle_special_var(t_gc *gc, const char **input);
char			*handle_empty_var(t_gc *gc);
char			*handle_normal_var(t_gc *gc, const char *start, size_t len);
int				is_whitespace(char c);
int				is_metachar(char c);
int				is_separator_metachar(char c);
int				should_concatenate(const char *current);
void			skip_whitespace(const char **input);

#endif
