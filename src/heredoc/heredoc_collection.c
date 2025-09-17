/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_collection.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexanfe <alexanfe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 00:00:00 by alexanfe          #+#    #+#             */
/*   Updated: 2025/09/17 00:00:00 by alexanfe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int	is_delimiter_match(char *line, char *delimiter)
{
	return (ft_strncmp(line, delimiter, ft_strlen(delimiter)) == 0
		&& ft_strlen(line) == ft_strlen(delimiter));
}

char	*append_line_to_content(t_gc *gc, char *content, char *line,
	size_t content_len)
{
	char	*new_content;
	size_t	line_len;

	line_len = ft_strlen(line);
	new_content = gc_malloc(gc, content_len + line_len + 2);
	if (!new_content)
		return (content);
	if (content_len > 0)
		ft_strlcpy(new_content, content, content_len + 1);
	else
		new_content[0] = '\0';
	ft_strlcat(new_content, line, content_len + line_len + 2);
	ft_strlcat(new_content, "\n", content_len + line_len + 2);
	return (new_content);
}

void	process_heredoc_line(char *line)
{
	size_t	line_len;

	line_len = ft_strlen(line);
	if (line_len > 0 && line[line_len - 1] == '\n')
		line[line_len - 1] = '\0';
}
