/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexanfe <alexanfe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/19 17:15:52 by alexanfe          #+#    #+#             */
/*   Updated: 2025/09/19 17:15:54 by alexanfe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTINS_H
# define BUILTINS_H

# include "env.h"

typedef enum e_builtin_type
{
	BUILTIN_NONE,
	BUILTIN_ECHO,
	BUILTIN_CD,
	BUILTIN_PWD,
	BUILTIN_ENV,
	BUILTIN_EXPORT,
	BUILTIN_UNSET,
	BUILTIN_EXIT
}	t_builtin_type;

t_builtin_type	is_builtin_command(const char *cmd_name);
int				execute_builtin(t_builtin_type type, char **args,
					t_shell_context *ctx);
int				builtin_echo(char **args);
int				builtin_cd(char **args, t_shell_context *ctx);
int				builtin_pwd(void);
int				builtin_env(t_shell_context *ctx);
int				builtin_export(char **args, t_shell_context *ctx);
int				builtin_unset(char **args, t_shell_context *ctx);
int				builtin_exit(char **args, t_shell_context *ctx);

#endif
