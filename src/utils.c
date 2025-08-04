/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elvictor <elvictor@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 17:33:12 by elvictor          #+#    #+#             */
/*   Updated: 2025/08/04 19:19:00 by elvictor         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//Wrappers

void	Getcwd(char *buf, size_t size)
{
	if (getcwd(buf, size) == NULL)
		perror("getcwd FAILED");
}
