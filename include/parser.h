/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexanfe <alexanfe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/19 17:17:59 by alexanfe          #+#    #+#             */
/*   Updated: 2025/09/19 17:18:00 by alexanfe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include "memory.h"
# include "tokenizer.h"
# include "ast.h"
# include "env.h"

typedef struct s_redir_ptrs
{
	t_redirection		*input_redirs;
	t_redirection		*output_redirs;
	t_redirection_entry	*redirections;
	int					redir_position;
}	t_redir_ptrs;

typedef struct s_parse_context
{
	char					**args;
	int						*i;
	int						max_args;
	t_redirection_entry		**redirections;
	int						redir_position;
	t_redirection			**input_redirs;
	t_redirection			**output_redirs;
}	t_parse_context;

typedef struct s_redir_params
{
	t_redirection_entry	**redirections;
	int					*redir_position;
	t_redirection		**input_redirs;
	t_redirection		**output_redirs;
	t_shell_env			*env;
}	t_redir_params;

typedef struct s_token_process_data
{
	char		**args;
	int			index;
	t_shell_env	*env;
}	t_token_process_data;

t_redirection_entry	*process_redirection_entry(t_gc *gc,
						t_token **tokens, int position);
t_redirection_entry	*process_heredoc_entry(t_gc *gc,
						t_token **tokens, int position);
t_ast_node			*parse(t_gc *gc, t_token *tokens, t_shell_env *env);
t_ast_node			*parse_pipe(t_gc *gc, t_token **tokens, t_shell_env *env);
t_ast_node			*parse_command(t_gc *gc,
						t_token **tokens, t_shell_env *env);
void				skip_redirections(t_token **current);
void				parse_redirections(t_gc *gc, t_token **tokens,
						t_redirection_entry **redirections,
						int *redir_position);
void				parse_redirections_legacy(t_gc *gc, t_token **tokens,
						t_redirection **input_redirs,
						t_redirection **output_redirs);
void				process_input_redirection(t_gc *gc, t_token **tokens,
						t_redirection **input_redirs);
void				process_heredoc_redirection(t_gc *gc, t_token **tokens,
						t_redirection **input_redirs);
void				process_output_redirection(t_gc *gc, t_token **tokens,
						t_redirection **output_redirs, t_token_type type);
void				process_word_token(t_gc *gc, t_token **tokens,
						t_parse_context *ctx, t_shell_env *env);
void				process_token(t_gc *gc, t_token **tokens,
						t_parse_context *ctx, t_shell_env *env);
void				init_redir_ptrs(t_redirection **input,
						t_redirection **output,
						t_redirection_entry **redirections,
						int *redir_position);
char				**extract_command_args(t_gc *gc, t_token **tokens,
						t_shell_env *env);
char				**extract_args_with_redirections(t_gc *gc,
						t_token **tokens, t_redir_params *params);
char				*expand_variable(t_gc *gc, const char *var_name,
						t_shell_env *env);
char				**expand_command_args(t_gc *gc, char **args,
						t_shell_env *env);
char				**extract_command_args(t_gc *gc, t_token **tokens,
						t_shell_env *env);
int					setup_redir_ordered(t_redirection_entry *redirections);
int					is_word_or_redirection_token(t_token_type type);
int					has_command_token(t_token *tokens);
int					is_redirection_token(t_token_type type);
int					count_command_tokens(t_token *tokens);
char				*process_regular_char(t_gc *gc, char **current, \
											char *result);
char				*append_to_result(t_gc *gc, char *result, char *append_str);
char				*extract_var_name(t_gc *gc, char *start);
void				free_split_array(char **array);
int					is_delimiter_match(char *line, char *delimiter);
char				*append_line_to_content(t_gc *gc, char *content, \
											char *line, size_t content_len);

#endif
