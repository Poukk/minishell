/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   concat.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexanfe <alexanfe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/20 21:58:53 by alexanfe          #+#    #+#             */
/*   Updated: 2025/09/20 21:58:55 by alexanfe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_concat_result	init_concat_result(void)
{
	t_concat_result	result;

	result.content = NULL;
	result.quote_context = 0;
	result.status = TOKENIZER_SUCCESS;
	return (result);
}

t_concat_result	extract_concatenated_content_with_context(t_gc *gc,
		const char **input)
{
	t_concat_result	result;

	result = init_concat_result();
	while (**input && should_concatenate(*input))
	{
		if (!process_single_content(gc, input, &result))
			return (result);
	}
	return (result);
}
