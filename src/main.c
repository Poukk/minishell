/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexanfe <alexanfe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 23:29:41 by alexanfe          #+#    #+#             */
/*   Updated: 2025/06/16 23:29:43 by alexanfe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <stdlib.h>

#define PROMPT "minishell$ "

static void	handle_line(char *line)
{
	t_gc	gc;
	t_token	*tokens;

	if (!line)
		return ;
	if (*line)
		add_history(line);
	gc_init(&gc);
	tokens = lexer_tokenize(&gc, line);
	if (tokens)
	{
		ft_printf("Tokenized input:\n");
		token_print_list(tokens);
	}
	else
		ft_printf("Error: Failed to tokenize input\n");
	gc_free_all(&gc);
}

int	main(void)
{
	char	*line;

	while (1)
	{
		line = readline(PROMPT);
		if (!line)
		{
			ft_printf("exit\n");
			break ;
		}
		handle_line(line);
		free(line);
	}
	return (0);
}
