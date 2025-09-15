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

typedef enum e_node_type
{
	NODE_CMD,
	NODE_PIPE
}	t_node_type;

typedef struct s_redirection
{
	int						type;
	char					*filename;
	struct s_redirection	*next;
}	t_redirection;

typedef struct s_parse_context
{
	char			**args;
	int				*i;
	int				max_args;
	t_redirection	**input_redirs;
	t_redirection	**output_redirs;
}	t_parse_context;

typedef struct s_ast_node
{
	t_node_type			type;
	char				**args;
	struct s_ast_node	*left;
	struct s_ast_node	*right;
	t_redirection		*input_redirs;
	t_redirection		*output_redirs;
}	t_ast_node;

//ast_redir.c
t_redirection	*redirection_create(t_gc *gc, int type, char *filename);
void			redirection_add_back(t_redirection **head,
					t_redirection *new_redir);
// gc.c
void			gc_init(t_gc *gc);
void			*gc_malloc(t_gc *gc, size_t size);
void			gc_free_all(t_gc *gc);

// lexer.c
t_token			*lexer_tokenize(t_gc *gc, const char *input);
char			*extract_word(t_gc *gc, const char **input);

// lexer_utils.c
t_token			*token_create(t_gc *gc, t_token_type type, const char *value);
void			token_add_back(t_token **head, t_token *new_token);
char			*extract_quoted_string(t_gc *gc,
					const char **input, char quote);
void			token_print_list(t_token *tokens);

// lexer_handler.c
t_token			*handle_metachar(t_gc *gc, const char **input);
t_token			*handle_word_or_quote(t_gc *gc, const char **input);

// ast.c
t_ast_node		*ast_node_create(t_gc *gc, t_node_type type);
void			ast_node_set_args(t_gc *gc, t_ast_node *node, char **args);

// ast_print.c
void			ast_print(t_ast_node *root, int depth);

// parser.c
t_ast_node		*parser_parse(t_gc *gc, t_token *tokens);
t_ast_node		*parse_pipe(t_gc *gc, t_token **tokens);
t_ast_node		*parse_command(t_gc *gc, t_token **tokens);

// parser_utils.c
char			**extract_command_args(t_gc *gc, t_token **tokens);
int				count_command_tokens(t_token *tokens);

// parser_redir.c
char			**extract_args_with_redirections(t_gc *gc, t_token **tokens,
					t_redirection **input_redirs,
					t_redirection **output_redirs);
void			parse_redirections(t_gc *gc, t_token **tokens,
					t_redirection **input_redirs,
					t_redirection **output_redirs);

// parser_redir_helpers.c
void			process_input_redirection(t_gc *gc, t_token **tokens,
					t_redirection **input_redirs);
void			process_output_redirection(t_gc *gc, t_token **tokens,
					t_redirection **output_redirs, t_token_type type);
void			process_word_token(t_gc *gc, t_token **tokens,
					t_parse_context *ctx);
void			process_token(t_gc *gc, t_token **tokens, t_parse_context *ctx);
int				is_redirection_token(t_token_type type);

// executor.c
int				executor_execute(t_ast_node *ast);
int				execute_command(char **args);
int				execute_command_with_redirections(t_ast_node *cmd_node);

// executor_path.c
char			*resolve_command_path(const char *command);
char			*get_validated_command_path(t_ast_node *cmd_node);

// executor_redir.c
int				setup_input_redirection(t_redirection *redir);
int				setup_output_redirection(t_redirection *redir);
int				create_all_output_files(t_redirection *output_redirs);

// executor_multi_redir.c
void			free_split_array(char **array);
int				setup_multiple_in_redirections(t_redirection *input_redirs);
int				setup_multiple_out_redirections(t_redirection *output_redirs);

// executor_pipes.c
int				execute_pipe(t_ast_node *ast);

#endif
