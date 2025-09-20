/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_path.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexanfe <alexanfe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 01:36:44 by alexanfe          #+#    #+#             */
/*   Updated: 2025/09/19 17:47:06 by alexanfe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_absolute_or_relative_path(const char *command)
{
	if (command[0] == '/' || (command[0] == '.' && command[1] == '/')
		|| (command[0] == '.' && command[1] == '.' && command[2] == '/'))
		return (1);
	return (0);
}

static char	*try_path_directory(const char *dir, const char *command)
{
	char	*full_path;
	int		dir_len;
	int		cmd_len;

	dir_len = ft_strlen(dir);
	cmd_len = ft_strlen(command);
	full_path = malloc(dir_len + cmd_len + 2);
	if (!full_path)
		return (NULL);
	ft_strlcpy(full_path, dir, dir_len + 1);
	if (dir_len > 0 && dir[dir_len - 1] != '/')
		ft_strlcat(full_path, "/", dir_len + 2);
	ft_strlcat(full_path, command, dir_len + cmd_len + 2);
	if (access(full_path, X_OK) == 0)
		return (full_path);
	free(full_path);
	return (NULL);
}

static char	*search_in_path_dirs(char **path_dirs, const char *command)
{
	char	*result;
	int		i;

	i = 0;
	while (path_dirs[i])
	{
		result = try_path_directory(path_dirs[i], command);
		if (result)
		{
			free_split_array(path_dirs);
			return (result);
		}
		i++;
	}
	free_split_array(path_dirs);
	return (NULL);
}

char	*resolve_command_path(const char *command)
{
	char		*path_env;
	char		**path_dirs;
	struct stat	path_stat;

	if (!command || !*command)
		return (NULL);
	if (is_absolute_or_relative_path(command))
	{
		if (access(command, F_OK) == 0)
		{
			if (stat(command, &path_stat) == 0 && S_ISDIR(path_stat.st_mode))
				return (NULL);
			return (ft_strdup(command));
		}
		return (NULL);
	}
	path_env = getenv("PATH");
	if (!path_env)
		return (NULL);
	path_dirs = ft_split(path_env, ':');
	if (!path_dirs)
		return (NULL);
	return (search_in_path_dirs(path_dirs, command));
}

char	*get_validated_command_path(t_ast_node *cmd_node)
{
	char	*command_path;

	if (!cmd_node || !cmd_node->args || !cmd_node->args[0])
		return (NULL);
	command_path = resolve_command_path(cmd_node->args[0]);
	if (!command_path)
		print_command_error(cmd_node->args[0], "command not found");
	return (command_path);
}
