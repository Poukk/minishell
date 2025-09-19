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

# include "gc.h"
# include "lexer.h"
# include "ast.h"
# include "env.h"

typedef struct s_parse_context
{
	char			**args;
	int				*i;
	int				max_args;
	t_redirection	**input_redirs;
	t_redirection	**output_redirs;
}	t_parse_context;

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

t_ast_node	*parse(t_gc *gc, t_token *tokens, t_shell_env *env);
t_ast_node	*parse_pipe(t_gc *gc, t_token **tokens, t_shell_env *env);
t_ast_node	*parse_command(t_gc *gc, t_token **tokens, t_shell_env *env);
void		skip_redirections(t_token **current);
void		parse_redirections(t_gc *gc, t_token **tokens,
				t_redirection **input_redirs,
				t_redirection **output_redirs);
void		process_input_redirection(t_gc *gc, t_token **tokens,
				t_redirection **input_redirs);
void		process_heredoc_redirection(t_gc *gc, t_token **tokens,
				t_redirection **input_redirs);
void		process_output_redirection(t_gc *gc, t_token **tokens,
				t_redirection **output_redirs, t_token_type type);
void		process_word_token(t_gc *gc, t_token **tokens,
				t_parse_context *ctx, t_shell_env *env);
void		process_token(t_gc *gc, t_token **tokens, t_parse_context *ctx,
				t_shell_env *env);
char		**extract_command_args(t_gc *gc, t_token **tokens,
				t_shell_env *env);
char		**extract_args_with_redirections(t_gc *gc,
				t_token **tokens, t_redir_params *params);
char		*expand_variable(t_gc *gc, const char *var_name,
				t_shell_env *env);
int			is_redirection_token(t_token_type type);
int			count_command_tokens(t_token *tokens);

#endif
