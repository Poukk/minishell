/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexanfe <alexanfe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/19 17:30:00 by alexanfe          #+#    #+#             */
/*   Updated: 2025/09/19 17:30:00 by alexanfe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTOR_H
# define EXECUTOR_H

# include "memory.h"
# include "ast.h"
# include "env.h"
# include <sys/types.h>

typedef struct s_cmd_setup
{
	char	**exp_args;
	char	*command_path;
	t_gc	*gc;
}	t_cmd_setup;

typedef struct s_expansion_state
{
	char	*content;
	char	*result;
	size_t	result_len;
	size_t	i;
	t_gc	*gc;
}	t_expansion_state;

typedef struct s_child_exec_ctx
{
	t_redirection_entry	*redirections;
	t_redirection	*input_redirs;
	t_redirection	*output_redirs;
	t_shell_env		*env;
}	t_child_exec_ctx;

int		execute(t_ast_node *ast, t_shell_context *ctx);
int		execute_command(char **args);
int		execute_command_with_redirections(t_ast_node *cmd_node,
			t_shell_context *ctx);
void	handle_execve_error(char **args, char *command_path);
int		wait_for_child(pid_t pid);
void	execute_child_process(char **args, char *command_path,
			t_child_exec_ctx *ctx);
void	init_empty_exec_ctx(t_child_exec_ctx *ctx);
void	execute_simple_command_child(char **args, char *command_path);
char	*expand_variable_in_string(t_gc *gc, char *str, t_shell_env *env);
char	*process_single_arg(t_gc *gc, char *arg, t_shell_env *env);
char	**expand_command_args(t_gc *gc, char **args, t_shell_env *env);
char	*append_to_result(t_gc *gc, char *result, char *append_str);
char	*extract_var_name(t_gc *gc, char *start);
char	*process_variable_char(t_gc *gc, char **current,
			t_shell_env *env, char *result);
char	*process_regular_char(t_gc *gc, char **current, char *result);
int		handle_command_setup(t_ast_node *cmd_node, t_shell_env *env,
			t_cmd_setup *setup);
char	*resolve_command_path(const char *command);
char	*get_validated_command_path(t_ast_node *cmd_node);
int		setup_input_redirection(t_redirection *redir);
int		setup_output_redirection(t_redirection *redir);
int		create_all_output_files(t_redirection *output_redirs);
int		execute_builtin_with_redirections(t_ast_node *cmd_node,
			t_shell_context *ctx);
int		execute_builtin_with_redirections_expanded(t_cmd_setup *setup,
			t_ast_node *cmd_node, t_shell_context *ctx);
int		is_delimiter_match(char *line, char *delimiter);
char	*append_line_to_content(t_gc *gc, char *content, char *line,
			size_t content_len);
void	process_heredoc_line(char *line);
char	*process_heredoc_loop(t_gc *gc, char *delimiter, char *content,
			int *line_count);
char	*collect_heredoc_content(t_gc *gc, char *delimiter);
char	*process_variable_expansion(t_gc *gc, char *content, size_t *i);
int		should_expand_variable(char *content, size_t i);
char	*expand_heredoc_variables(t_gc *gc, char *content, char *delimiter);
char	*init_expansion_result(t_gc *gc);
char	*append_variable_to_result(t_gc *gc, char *result,
			char *var_value, size_t result_len);
char	*append_char_to_result(t_gc *gc, char *result,
			char c, size_t result_len);
void	handle_double_dollar(t_expansion_state *state);
void	handle_regular_char(t_expansion_state *state);
void	handle_variable_expansion(t_expansion_state *state);
char	*process_expansion_loop(t_gc *gc, char *content, char *result);
int		setup_heredoc_redirection(t_redirection *redir);
void	free_split_array(char **array);
int		setup_multiple_in_redirections(t_redirection *input_redirs);
int		setup_multiple_out_redirections(t_redirection *output_redirs);
int		execute_pipe(t_ast_node *ast, t_shell_context *ctx);
int		handle_file_open_error(char *filename);

#endif
