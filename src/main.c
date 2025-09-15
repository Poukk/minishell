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

static void	execute_ast(t_ast_node *ast)
{
	int	exit_code;

	if (ast)
	{
		ft_printf("AST Structure:\n");
		ast_print(ast, 0);
		ft_printf("\nExecuting command...\n");
		exit_code = executor_execute(ast);
		ft_printf("Command completed with exit code: %d\n", exit_code);
	}
	else
		ft_printf("Syntax error: Invalid command structure\n");
}

static void	handle_line(char *line)
{
	t_gc		gc;
	t_token		*tokens;
	t_ast_node	*ast;

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
		ft_printf("\n");
		ast = parser_parse(&gc, tokens);
		execute_ast(ast);
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
