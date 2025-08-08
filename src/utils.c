/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elvictor <elvictor@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 17:33:12 by elvictor          #+#    #+#             */
/*   Updated: 2025/08/08 14:42:48 by elvictor         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//Wrappers

void	get_cwd(char *buf, size_t size)
{
	if (getcwd(buf, size) == NULL)
		perror("getcwd FAILED");
}
