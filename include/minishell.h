/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexanfe <alexanfe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 23:29:54 by alexanfe          #+#    #+#             */
/*   Updated: 2025/06/16 23:29:59 by alexanfe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdlib.h>
# include <stdio.h>
# include <unistd.h>
# include <string.h>

# include <readline/history.h>
# include <readline/readline.h>

# define SUCCESS 0
# define FAILURE 1

typedef enum e_bool
{
	FALSE,
	TRUE
}	t_bool;

void	get_cwd(char *buf, size_t size);
void	free_tokens(char **tokens);
//void	*malloc_util(size_t size);
//void	*realloc_util(void *ptr, size_t size);

//BUILTINS
t_bool	is_builtin(const char *cmd);
int		exec_builtin(char **args, int last_status);


#endif
