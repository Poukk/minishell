/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexanfe <alexanfe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 01:22:42 by alexanfe          #+#    #+#             */
/*   Updated: 2025/09/02 01:22:43 by alexanfe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*process_variable_token(t_gc *gc, t_token *token, t_shell_env *env)
{
	char	*var_with_dollar;
	size_t	len;

	(void)env;
	len = ft_strlen(token->value) + 2;
	var_with_dollar = gc_malloc(gc, len);
	if (!var_with_dollar)
		return (NULL);
	var_with_dollar[0] = '$';
	ft_strlcpy(var_with_dollar + 1, token->value, len - 1);
	return (var_with_dollar);
}

static char	*process_word_token_internal(t_gc *gc, t_token *token)
{
	char	*arg;

	arg = gc_malloc(gc, ft_strlen(token->value) + 1);
	if (!arg)
		return (NULL);
	ft_strlcpy(arg, token->value, ft_strlen(token->value) + 1);
	return (arg);
}

static char	**allocate_args_array(t_gc *gc, int count)
{
	char	**args;

	if (count == 0)
		return (NULL);
	args = gc_malloc(gc, sizeof(char *) * (count + 1));
	return (args);
}

static int	process_token_to_args(t_gc *gc, t_token **tokens,
		t_token_process_data *data)
{
	if ((*tokens)->type == TOKEN_VARIABLE)
		data->args[data->index] = process_variable_token(gc, *tokens,
				data->env);
	else
		data->args[data->index] = process_word_token_internal(gc, *tokens);
	if (!data->args[data->index])
		return (-1);
	*tokens = (*tokens)->next;
	skip_redirections(tokens);
	return (0);
}

char	**extract_command_args(t_gc *gc, t_token **tokens, t_shell_env *env)
{
	t_token_process_data	data;
	int						count;
	int						i;

	count = count_command_tokens(*tokens);
	data.args = allocate_args_array(gc, count);
	if (!data.args)
		return (NULL);
	data.env = env;
	i = 0;
	while (i < count && *tokens && ((*tokens)->type == TOKEN_WORD
			|| (*tokens)->type == TOKEN_VARIABLE))
	{
		data.index = i;
		if (process_token_to_args(gc, tokens, &data) == -1)
			return (NULL);
		i++;
	}
	data.args[i] = NULL;
	return (data.args);
}
