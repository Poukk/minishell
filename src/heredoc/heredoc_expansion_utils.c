/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_expansion_utils.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexanfe <alexanfe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 00:00:00 by alexanfe          #+#    #+#             */
/*   Updated: 2025/09/17 00:00:00 by alexanfe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*init_expansion_result(t_gc *gc)
{
	char	*result;

	result = gc_malloc(gc, 1);
	if (!result)
		return (NULL);
	result[0] = '\0';
	return (result);
}

char	*append_variable_to_result(t_gc *gc, char *result,
	char *var_value, size_t result_len)
{
	char	*new_result;

	if (!var_value || ft_strlen(var_value) == 0)
		return (result);
	new_result = gc_malloc(gc, result_len + ft_strlen(var_value) + 1);
	if (!new_result)
		return (result);
	if (result_len > 0)
		ft_strlcpy(new_result, result, result_len + 1);
	else
		new_result[0] = '\0';
	ft_strlcat(new_result, var_value, result_len + ft_strlen(var_value) + 1);
	return (new_result);
}

char	*append_char_to_result(t_gc *gc, char *result,
	char c, size_t result_len)
{
	char	*new_result;

	new_result = gc_malloc(gc, result_len + 2);
	if (!new_result)
		return (result);
	if (result_len > 0)
		ft_strlcpy(new_result, result, result_len + 1);
	else
		new_result[0] = '\0';
	new_result[result_len] = c;
	new_result[result_len + 1] = '\0';
	return (new_result);
}

void	handle_double_dollar(t_expansion_state *state)
{
	state->result = append_char_to_result(state->gc, state->result, '$',
			state->result_len);
	state->result_len++;
	state->i += 2;
}

void	handle_regular_char(t_expansion_state *state)
{
	state->result = append_char_to_result(state->gc, state->result,
			state->content[state->i], state->result_len);
	state->result_len++;
	state->i++;
}
