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

int	count_command_tokens(t_token *tokens)
{
	int		count;
	t_token	*current;

	count = 0;
	current = tokens;
	while (current && current->type == TOKEN_WORD)
	{
		count++;
		current = current->next;
	}
	return (count);
}

char	**extract_command_args(t_gc *gc, t_token **tokens)
{
	char	**args;
	int		count;
	int		i;

	count = count_command_tokens(*tokens);
	if (count == 0)
		return (NULL);
	args = gc_malloc(gc, sizeof(char *) * (count + 1));
	if (!args)
		return (NULL);
	i = 0;
	while (i < count && *tokens && (*tokens)->type == TOKEN_WORD)
	{
		args[i] = gc_malloc(gc, ft_strlen((*tokens)->value) + 1);
		if (!args[i])
			return (NULL);
		ft_strlcpy(args[i], (*tokens)->value, ft_strlen((*tokens)->value) + 1);
		*tokens = (*tokens)->next;
		i++;
	}
	args[i] = NULL;
	return (args);
}
