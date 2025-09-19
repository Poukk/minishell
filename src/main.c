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
#include <readline/readline.h>
#include <readline/history.h>
#include <stdlib.h>

#define PROMPT "minishell$ "

static void	execute_ast(t_ast_node *ast, t_shell_context *ctx)
{
	int	exit_code;

	if (ast)
	{
		ft_printf("AST Structure:\n");
		ast_print(ast, 0);
		ft_printf("\nExecuting command...\n");
		exit_code = executor_execute(ast, ctx);
		env_set_exit_code(ctx->env, exit_code);
		ft_printf("Command completed with exit code: %d\n", exit_code);
	}
	else
		ft_printf("Syntax error: Invalid command structure\n");
}

static void	handle_line(char *line, t_shell_context *ctx)
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
		ast = parser_parse(&gc, tokens, ctx->env);
		execute_ast(ast, ctx);
	}
	else
		ft_printf("Error: Failed to tokenize input\n");
	gc_free_all(&gc);
}

static t_shell_env	*init_shell_env(t_gc *gc, char **envp)
{
	t_shell_env	*env;

	env = env_init_from_system(gc, envp);
	if (!env)
	{
		ft_printf("Error: Failed to initialize environment\n");
		gc_free_all(gc);
		exit(1);
	}
	return (env);
}

static void	main_loop(t_shell_context *ctx)
{
	char	*line;
	int		signal_exit_code;

	while (1)
	{
		line = readline(PROMPT);
		if (get_signal_received())
		{
			signal_exit_code = 128 + get_signal_received();
			env_set_exit_code(ctx->env, signal_exit_code);
			reset_signal_received();
			if (line)
				free(line);
			continue ;
		}
		if (!line)
		{
			ft_printf("exit\n");
			break ;
		}
		handle_line(line, ctx);
		free(line);
	}
}

int	main(int argc, char **argv, char **envp)
{
	t_gc			gc;
	t_shell_env		*env;
	t_shell_context	ctx;

	(void)argc;
	(void)argv;
	setup_shell_signals();
	gc_init(&gc);
	env = init_shell_env(&gc, envp);
	ctx.env = env;
	ctx.gc = &gc;
	main_loop(&ctx);
	gc_free_all(&gc);
	return (env->last_exit_code);
}
