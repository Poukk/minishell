/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexanfe <alexanfe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/19 18:00:00 by alexanfe          #+#    #+#             */
/*   Updated: 2025/09/19 18:00:01 by alexanfe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DEBUG_H
# define DEBUG_H

# include "../include/minishell.h"

// Debug printing functions (non-production)
void			token_print_list(t_token *tokens);
void			ast_print(t_ast_node *root, int depth);

#endif