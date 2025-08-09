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

//#define	DEL "\n\t \v\f\r"

void	get_cwd(char *buf, size_t size);
void	free_tokens(char **tokens);
//void	*malloc_util(size_t size);
//void	*realloc_util(void *ptr, size_t size);



#endif
