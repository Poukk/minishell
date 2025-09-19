/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_print.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexanfe <alexanfe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 01:22:21 by alexanfe          #+#    #+#             */
/*   Updated: 2025/09/02 01:22:23 by alexanfe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "debug.h"

static void	print_indent(int depth)
{
	int	i;

	i = 0;
	while (i < depth)
	{
		ft_printf("  ");
		i++;
	}
}

static void	print_node_type(t_node_type type)
{
	if (type == NODE_CMD)
		ft_printf("CMD");
	else if (type == NODE_PIPE)
		ft_printf("PIPE");
}

static void	print_args(char **args)
{
	int	i;

	if (!args)
		return ;
	ft_printf(" [");
	i = 0;
	while (args[i])
	{
		ft_printf("%s", args[i]);
		if (args[i + 1])
			ft_printf(", ");
		i++;
	}
	ft_printf("]");
}

void	ast_print(t_ast_node *root, int depth)
{
	if (!root)
		return ;
	print_indent(depth);
	print_node_type(root->type);
	print_args(root->args);
	ft_printf("\n");
	if (root->left)
		ast_print(root->left, depth + 1);
	if (root->right)
		ast_print(root->right, depth + 1);
}
