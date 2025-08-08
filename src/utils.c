/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elvictor <elvictor@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 17:33:12 by elvictor          #+#    #+#             */
/*   Updated: 2025/08/08 17:59:29 by elvictor         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//Wrappers

void	get_cwd(char *buf, size_t size)
{
	if (getcwd(buf, size) == NULL)
		perror("getcwd FAILED");
}

void *malloc_util(size_t size)
{
	void *ptr;
	
	if (size == 0)
		return (NULL);
	ptr = malloc(size);
	if (!ptr)
	{
		perror("malloc_util failed");
		exit(EXIT_FAILURE);
	}
	return (ptr);
}

void	*realloc_util(void *ptr, size_t size)
{
	void	*new_ptr;
	
	new_ptr = realloc(ptr, size);
	if (!new_ptr && size != 0)
	{
		perror("realloc_util failed!");
		exit(EXIT_FAILURE);
	}
	return (new_ptr);
}