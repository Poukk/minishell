/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexanfe <alexanfe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 23:29:54 by alexanfe          #+#    #+#             */
/*   Updated: 2025/09/17 02:05:00 by alexanfe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft.h"
# include <stdlib.h>
# include <stdio.h>
# include <errno.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <readline/readline.h>

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
	TOKEN_VARIABLE,
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
	char					*heredoc_content;
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

typedef struct s_expansion_state
{
	char	*content;
	char	*result;
	size_t	result_len;
	size_t	i;
	t_gc	*gc;
}	t_expansion_state;

typedef struct s_env_var
{
	char				*key;
	char				*value;
	struct s_env_var	*next;
}	t_env_var;

typedef struct s_shell_env
{
	t_env_var	*vars;
	int			last_exit_code;
	char		*cwd;
}	t_shell_env;

typedef struct s_shell_context
{
	t_shell_env	*env;
	t_gc		*gc;
}	t_shell_context;

typedef struct s_redir_params
{
	t_redirection	**input_redirs;
	t_redirection	**output_redirs;
	t_shell_env		*env;
}	t_redir_params;

typedef struct s_token_process_data
{
	char		**args;
	int			index;
	t_shell_env	*env;
}	t_token_process_data;

//ast_redir.c
t_redirection	*redirection_create(t_gc *gc, int type, char *filename);
t_redirection	*heredoc_redirection_create(t_gc *gc, char *delimiter,
					char *content);
void			redirection_add_back(t_redirection **head,
					t_redirection *new_redir);
// gc.c
void			gc_init(t_gc *gc);
void			*gc_malloc(t_gc *gc, size_t size);
void			gc_free_all(t_gc *gc);
char			*gc_readline(t_gc *gc, const char *prompt);

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
t_ast_node		*parser_parse(t_gc *gc, t_token *tokens, t_shell_env *env);
t_ast_node		*parse_pipe(t_gc *gc, t_token **tokens, t_shell_env *env);
t_ast_node		*parse_command(t_gc *gc, t_token **tokens, t_shell_env *env);

// parser_utils.c
char			**extract_command_args(t_gc *gc, t_token **tokens,
					t_shell_env *env);

// parser_redir.c
char			**extract_args_with_redirections(t_gc *gc,
					t_token **tokens, t_redir_params *params);
void			parse_redirections(t_gc *gc, t_token **tokens,
					t_redirection **input_redirs,
					t_redirection **output_redirs);

// parser_redir_helpers.c
void			process_input_redirection(t_gc *gc, t_token **tokens,
					t_redirection **input_redirs);
void			process_heredoc_redirection(t_gc *gc, t_token **tokens,
					t_redirection **input_redirs);
void			process_output_redirection(t_gc *gc, t_token **tokens,
					t_redirection **output_redirs, t_token_type type);
void			process_word_token(t_gc *gc, t_token **tokens,
					t_parse_context *ctx, t_shell_env *env);
void			process_token(t_gc *gc, t_token **tokens, t_parse_context *ctx,
					t_shell_env *env);
int				is_redirection_token(t_token_type type);
int				count_command_tokens(t_token *tokens);
void			skip_redirections(t_token **current);

// parser_expansion.c
char			*expand_variable(t_gc *gc, const char *var_name,
					t_shell_env *env);

typedef struct s_cmd_setup
{
	char	**expanded_args;
	char	*command_path;
	t_gc	*gc;
}	t_cmd_setup;

// executor.c
int				executor_execute(t_ast_node *ast, t_shell_env *env);
int				execute_command(char **args);
int				execute_command_with_redirections(t_ast_node *cmd_node,
					t_shell_env *env);

// executor_utils.c
void			handle_execve_error(char **args, char *command_path);
int				wait_for_child(pid_t pid);

// executor_expansion.c
int				count_args(char **args);
char			*process_single_arg(t_gc *gc, char *arg, t_shell_env *env);
char			**expand_command_args(t_gc *gc, char **args, t_shell_env *env);
int				handle_command_setup(t_ast_node *cmd_node, t_shell_env *env,
					t_cmd_setup *setup);

// executor_path.c
char			*resolve_command_path(const char *command);
char			*get_validated_command_path(t_ast_node *cmd_node);

// executor_redir.c
int				setup_input_redirection(t_redirection *redir);
int				setup_output_redirection(t_redirection *redir);
int				create_all_output_files(t_redirection *output_redirs);

// heredoc_collection.c
int				is_delimiter_match(char *line, char *delimiter);
char			*append_line_to_content(t_gc *gc, char *content, char *line,
					size_t content_len);
void			process_heredoc_line(char *line);

// heredoc_main.c
char			*process_heredoc_loop(t_gc *gc, char *delimiter, char *content,
					int *line_count);
char			*collect_heredoc_content(t_gc *gc, char *delimiter);

// heredoc_expansion_vars.c
char			*process_variable_expansion(t_gc *gc, char *content, size_t *i);
int				should_expand_variable(char *content, size_t i);
char			*expand_heredoc_variables(t_gc *gc, char *content,
					char *delimiter);

// heredoc_expansion_utils.c
char			*init_expansion_result(t_gc *gc);
char			*append_variable_to_result(t_gc *gc, char *result,
					char *var_value, size_t result_len);
char			*append_char_to_result(t_gc *gc, char *result,
					char c, size_t result_len);
void			handle_double_dollar(t_expansion_state *state);
void			handle_regular_char(t_expansion_state *state);

// heredoc_expansion_loop.c
void			handle_variable_expansion(t_expansion_state *state);
char			*process_expansion_loop(t_gc *gc, char *content, char *result);

// heredoc_redirection.c
int				setup_heredoc_redirection(t_redirection *redir);

// executor_multi_redir.c
void			free_split_array(char **array);
int				setup_multiple_in_redirections(t_redirection *input_redirs);
int				setup_multiple_out_redirections(t_redirection *output_redirs);

// executor_pipes.c
int				execute_pipe(t_ast_node *ast, t_shell_env *env);

// error.c
int				handle_file_open_error(char *filename);

// env_storage.c
t_shell_env		*env_create(t_gc *gc);
t_env_var		*env_find_var(t_shell_env *env, const char *key);
int				env_set_var(t_gc *gc, t_shell_env *env, const char *key,
					const char *value);
int				env_unset_var(t_shell_env *env, const char *key);
char			*env_get_value(t_shell_env *env, const char *key);

// env_init.c
t_shell_env		*env_init_from_system(t_gc *gc, char **envp);
void			env_update_pwd(t_gc *gc, t_shell_env *env);
void			env_set_exit_code(t_shell_env *env, int exit_code);

// env_utils.c
char			**env_to_array(t_gc *gc, t_shell_env *env);
void			env_print_all(t_shell_env *env);
int				env_is_valid_name(const char *name);

#endif
