/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexanfe <alexanfe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 01:22:27 by alexanfe          #+#    #+#             */
/*   Updated: 2025/09/02 01:22:28 by alexanfe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_ast_node	*ast_node_create(t_gc *gc, t_node_type type)
{
	t_ast_node	*node;

	node = gc_malloc(gc, sizeof(t_ast_node));
	if (!node)
		return (NULL);
	node->type = type;
	node->args = NULL;
	node->left = NULL;
	node->right = NULL;
	return (node);
}

static int	count_args(char **args)
{
	int	count;

	count = 0;
	if (!args)
		return (0);
	while (args[count])
		count++;
	return (count);
}

static char	**copy_args(t_gc *gc, char **args)
{
	char	**new_args;
	int		i;
	int		len;

	len = count_args(args);
	if (len == 0)
		return (NULL);
	new_args = gc_malloc(gc, sizeof(char *) * (len + 1));
	if (!new_args)
		return (NULL);
	i = 0;
	while (i < len)
	{
		new_args[i] = gc_malloc(gc, ft_strlen(args[i]) + 1);
		if (!new_args[i])
			return (NULL);
		ft_strlcpy(new_args[i], args[i], ft_strlen(args[i]) + 1);
		i++;
	}
	new_args[i] = NULL;
	return (new_args);
}

void	ast_node_set_args(t_gc *gc, t_ast_node *node, char **args)
{
	if (!node)
		return ;
	node->args = copy_args(gc, args);
}
