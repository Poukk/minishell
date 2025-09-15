/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_multi_redir.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexanfe <alexanfe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 01:36:44 by alexanfe          #+#    #+#             */
/*   Updated: 2025/09/15 01:36:45 by alexanfe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdlib.h>

void	free_split_array(char **array)
{
	int	i;

	if (!array)
		return ;
	i = 0;
	while (array[i])
	{
		free(array[i]);
		i++;
	}
	free(array);
}

int	setup_multiple_input_redirections(t_redirection *input_redirs)
{
	t_redirection	*current;

	if (!input_redirs)
		return (0);
	current = input_redirs;
	while (current->next)
		current = current->next;
	return (setup_input_redirection(current));
}

int	setup_multiple_output_redirections(t_redirection *output_redirs)
{
	t_redirection	*current;

	if (!output_redirs)
		return (0);
	if (create_all_output_files(output_redirs) == -1)
		return (-1);
	current = output_redirs;
	while (current->next)
		current = current->next;
	return (setup_output_redirection(current));
}
