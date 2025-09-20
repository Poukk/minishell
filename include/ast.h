/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexanfe <alexanfe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/19 17:13:49 by alexanfe          #+#    #+#             */
/*   Updated: 2025/09/19 17:15:32 by alexanfe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AST_H
# define AST_H

# include "memory.h"

typedef enum e_node_type
{
	NODE_CMD,
	NODE_PIPE
}	t_node_type;

typedef struct s_redirection_entry
{
	int							type;
	char						*filename;
	char						*heredoc_content;
	int							position;
	struct s_redirection_entry	*next;
}	t_redirection_entry;

typedef struct s_redirection
{
	int						type;
	char					*filename;
	char					*heredoc_content;
	struct s_redirection	*next;
}	t_redirection;

typedef struct s_ast_node
{
	t_node_type				type;
	char					**args;
	struct s_ast_node		*left;
	struct s_ast_node		*right;
	t_redirection_entry		*redirections;
	t_redirection			*input_redirs;
	t_redirection			*output_redirs;
}	t_ast_node;

t_redirection_entry	*redirection_entry_create(t_gc *gc, int type, char *filename, int position);
t_redirection_entry	*heredoc_entry_create(t_gc *gc, char *delimiter,
						char *content, int position);
void				redirection_entry_add_ordered(t_redirection_entry **head,
						t_redirection_entry *new_entry);
t_redirection_entry	*redirection_entry_get_by_position(t_redirection_entry *head,
						int position);

t_redirection		*redirection_create(t_gc *gc, int type, char *filename);
t_redirection		*heredoc_redirection_create(t_gc *gc, char *delimiter,
						char *content);
t_ast_node			*ast_node_create(t_gc *gc, t_node_type type);
void				ast_node_set_args(t_gc *gc, t_ast_node *node, char **args);
void				redirection_add_back(t_redirection **head,
						t_redirection *new_redir);

#endif
