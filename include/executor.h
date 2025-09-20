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
	t_redirection		*input_redirs;
	t_redirection		*output_redirs;
	t_shell_env			*env;
}	t_child_exec_ctx;

typedef struct s_external_exec_data
{
	t_cmd_setup		*setup;
	t_ast_node		*cmd_node;
	t_gc			*gc;
	t_shell_context	*ctx;
}	t_external_exec_data;

char	*resolve_command_path(const char *command, t_shell_env *env);
char	*get_validated_command_path(t_ast_node *cmd_node, t_shell_env *env);
void	init_empty_exec_ctx(t_child_exec_ctx *ctx);
void	execute_simple_command_child(char **args, char *command_path);
void	handle_execve_error(char **args, char *command_path);
void	execute_child_process(char **args, char *command_path,
			t_child_exec_ctx *ctx);
void	setup_child_exec_context(t_child_exec_ctx *exec_ctx,
			t_ast_node *cmd_node, t_shell_context *ctx);
int		execute_cmd(t_ast_node *cmd_node, t_shell_context *ctx);
int		execute(t_ast_node *ast, t_shell_context *ctx);
int		wait_for_child(pid_t pid);
int		check_directory_and_fork(char *command_path, char **expanded_args,
			t_gc *gc);
int		handle_command_setup(t_ast_node *cmd_node, t_shell_env *env,
			t_cmd_setup *setup);
int		setup_and_validate_command(t_ast_node *cmd_node, t_shell_env *env,
			t_cmd_setup *setup, t_gc *gc);
int		execute_builtin_with_redirections(t_ast_node *cmd_node,
			t_shell_context *ctx);
int		execute_builtin_with_redirections_expanded(t_cmd_setup *setup,
			t_ast_node *cmd_node, t_shell_context *ctx);
int	wait_for_pipe_children(pid_t left_pid, pid_t right_pid,
			int pipefd[2]);
int		execute_pipe(t_ast_node *ast, t_shell_context *ctx);
int		setup_multiple_in_redirections(t_redirection *input_redirs);
int		setup_multiple_out_redirections(t_redirection *output_redirs);
int	setup_pipe_and_signals(int pipefd[2]);

char	*init_expansion_result(t_gc *gc);
char	*process_expansion_loop(t_gc *gc, char *content, char *result);
char	*process_variable_expansion(t_gc *gc, char *content, size_t *i);
char	*append_variable_to_result(t_gc *gc, char *result,
	char *var_value, size_t result_len);
void	handle_double_dollar(t_expansion_state *state);
int	should_expand_variable(char *content, size_t i);
void	handle_regular_char(t_expansion_state *state);
void	process_heredoc_line(char *line);
int	setup_heredoc_redirection(t_redirection *redir);
int	setup_input_redirection(t_redirection *redir);
int	setup_output_redirection(t_redirection *redir);
int	create_all_output_files(t_redirection *output_redirs);


#endif
