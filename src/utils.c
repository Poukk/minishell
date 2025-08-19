/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elvictor <elvictor@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 17:33:12 by elvictor          #+#    #+#             */
/*   Updated: 2025/08/19 17:53:33 by elvictor         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*

//Wrappers

void	get_cwd(char *buf, size_t size)
{
	if (getcwd(buf, size) == NULL)
		perror("getcwd FAILED");
}

void	free_tokens(char **tokens)
{
	int	i;

	i = 0;
	if (!tokens)
		return ;
	while (tokens[i])
	{
		free(tokens[i]);
		i++;
	}
	free(tokens);
}
*/