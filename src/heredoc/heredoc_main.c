/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_main.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexanfe <alexanfe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 00:00:00 by alexanfe          #+#    #+#             */
/*   Updated: 2025/09/17 00:00:00 by alexanfe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdio.h>

static char	*init_heredoc_content(t_gc *gc)
{
	char	*content;

	content = gc_malloc(gc, 1);
	if (!content)
		return (NULL);
	content[0] = '\0';
	return (content);
}

char	*process_heredoc_loop(t_gc *gc, char *delimiter, char *content,
	int *line_count)
{
	char	*line;
	size_t	content_len;

	content_len = 0;
	line = gc_readline(gc, "> ");
	while (line != NULL)
	{
		process_heredoc_line(line);
		if (is_delimiter_match(line, delimiter))
			break ;
		content = append_line_to_content(gc, content, line, content_len);
		content_len = ft_strlen(content);
		(*line_count)++;
		line = gc_readline(gc, "> ");
	}
	return (content);
}

char	*collect_heredoc_content(t_gc *gc, char *delimiter)
{
	char	*content;
	int		line_count;

	content = init_heredoc_content(gc);
	if (!content)
		return (NULL);
	line_count = 0;
	content = process_heredoc_loop(gc, delimiter, content, &line_count);
	if (line_count > 0)
		ft_dprintf(2, "minishell: heredoc at line %d delimited by EOF \n",
			line_count);
	return (content);
}

void	process_heredoc_redirection(t_gc *gc, t_token **tokens,
		t_redirection **input_redirs)
{
	t_redirection	*new_redir;
	char			*content;

	*tokens = (*tokens)->next;
	if (!*tokens || (*tokens)->type != TOKEN_WORD)
	{
		ft_dprintf(2,
			"minishell: syntax error near unexpected token `newline'\n");
		return ;
	}
	content = collect_heredoc_content(gc, (*tokens)->value);
	if (content)
		content = expand_heredoc_variables(gc, content, (*tokens)->value);
	new_redir = heredoc_redirection_create(gc, (*tokens)->value, content);
	if (new_redir)
		redirection_add_back(input_redirs, new_redir);
	*tokens = (*tokens)->next;
}
