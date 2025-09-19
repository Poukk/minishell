/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_loop.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexanfe <alexanfe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/19 17:47:37 by alexanfe          #+#    #+#             */
/*   Updated: 2025/09/19 17:47:37 by alexanfe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_variable_expansion(t_expansion_state *state)
{
	char	*var_value;

	var_value = process_variable_expansion(state->gc, state->content,
			&state->i);
	state->result = append_variable_to_result(state->gc, state->result,
			var_value, state->result_len);
	state->result_len = ft_strlen(state->result);
	state->i++;
}

char	*process_expansion_loop(t_gc *gc, char *content, char *result)
{
	t_expansion_state	state;

	state.gc = gc;
	state.content = content;
	state.result = result;
	state.result_len = 0;
	state.i = 0;
	while (content[state.i])
	{
		if (content[state.i] == '$' && content[state.i + 1] == '$')
			handle_double_dollar(&state);
		else if (should_expand_variable(content, state.i))
			handle_variable_expansion(&state);
		else
			handle_regular_char(&state);
	}
	return (state.result);
}
