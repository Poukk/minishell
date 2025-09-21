/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexanfe <alexanfe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/19 19:41:50 by alexanfe          #+#    #+#             */
/*   Updated: 2025/09/19 19:41:53 by alexanfe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexanfe <alexanfe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 23:29:41 by alexanfe          #+#    #+#             */
/*   Updated: 2025/09/19 17:02:12 by alexanfe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	execute_ast(t_ast_node *ast, t_shell_context *ctx)
{
	int	exit_code;

	if (ast)
	{
		exit_code = execute(ast, ctx);
		env_set_exit_code(ctx->env, exit_code);
	}
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
	tokens = tokenize(&gc, line);
	if (tokens)
	{
		ast = parse(&gc, tokens, ctx->env);
		execute_ast(ast, ctx);
	}
	gc_free_all(&gc);
}

static t_shell_env	*init_shell_env(t_gc *gc, char **envp)
{
	t_shell_env	*env;

	env = env_init_from_system(gc, envp);
	if (!env)
	{
		gc_free_all(gc);
		exit(1);
	}
	return (env);
}

static void	main_loop(t_shell_context *ctx)
{
	char			*line;
	int				signal_exit_code;
	struct termios	term;

	tcgetattr(STDIN_FILENO, &term);
	while (1)
	{
		tcsetattr(STDIN_FILENO, TCSANOW, &term);
		line = readline(PROMPT);
		if (!line)
		{
			ft_printf("exit\n");
			break ;
		}
		if (get_signal_received())
		{
			signal_exit_code = 128 + get_signal_received();
			env_set_exit_code(ctx->env, signal_exit_code);
			reset_signal_received();
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
	int				last_exit_code;

	(void)argc;
	(void)argv;
	setup_shell_signals();
	gc_init(&gc);
	env = init_shell_env(&gc, envp);
	ctx.env = env;
	ctx.gc = &gc;
	main_loop(&ctx);
	last_exit_code = env->last_exit_code;
	gc_free_all(&gc);
	return (last_exit_code);
}
