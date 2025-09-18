/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_print.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexanfe <alexanfe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 19:47:37 by alexanfe          #+#    #+#             */
/*   Updated: 2025/09/18 19:47:38 by alexanfe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	env_print_all(t_shell_env *env)
{
	t_env_var	*current;

	if (!env)
		return ;
	current = env->vars;
	while (current)
	{
		ft_printf("%s=%s\n", current->key, current->value);
		current = current->next;
	}
}
