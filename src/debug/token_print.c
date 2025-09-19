/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_print.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexanfe <alexanfe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/19 18:00:00 by alexanfe          #+#    #+#             */
/*   Updated: 2025/09/19 18:00:01 by alexanfe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "debug.h"

void	token_print_list(t_token *tokens)
{
	const char	*type_names[] = {
		"WORD", "PIPE", "REDIR_IN", "REDIR_OUT",
		"REDIR_APPEND", "HEREDOC", "VARIABLE", "EOF"
	};

	while (tokens)
	{
		ft_printf("Token: %s", type_names[tokens->type]);
		if (tokens->value)
			ft_printf(" | Value: '%s'", tokens->value);
		ft_printf("\n");
		tokens = tokens->next;
	}
}
