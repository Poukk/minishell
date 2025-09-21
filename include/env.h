/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexanfe <alexanfe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/19 17:30:00 by alexanfe          #+#    #+#             */
/*   Updated: 2025/09/19 17:30:00 by alexanfe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENV_H
# define ENV_H

# include "memory.h"

typedef struct s_env_var
{
	char				*key;
	char				*value;
	struct s_env_var	*next;
}	t_env_var;

typedef struct s_shell_env
{
	t_env_var	*vars;
	int			last_exit_code;
	char		*cwd;
}	t_shell_env;

typedef struct s_shell_context
{
	t_shell_env	*env;
	t_gc		*gc;
}	t_shell_context;

t_shell_env	*env_create(t_gc *gc);
t_shell_env	*env_init_from_system(t_gc *gc, char **envp);
t_env_var	*env_find_var(t_shell_env *env, const char *key);
void		env_update_pwd(t_gc *gc, t_shell_env *env);
void		env_set_exit_code(t_shell_env *env, int exit_code);
void		env_print_all(t_shell_env *env);
char		*env_get_value(t_shell_env *env, const char *key);
char		**env_to_array(t_gc *gc, t_shell_env *env);
char		*process_variable_char(t_gc *gc, char **current, t_shell_env *env, \
									char *result);
int			env_unset_var(t_shell_env *env, const char *key);
int			env_is_valid_name(const char *name);
int			env_set_var(t_gc *gc, t_shell_env *env, const char *key,
				const char *value);
#endif
