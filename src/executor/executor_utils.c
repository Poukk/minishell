/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexanfe <alexanfe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/07 01:36:44 by alexanfe          #+#    #+#             */
/*   Updated: 2025/09/07 01:36:45 by alexanfe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>

static void	free_split_array(char **array)
{
	int	i;

	if (!array)
		return ;
	i = 0;
	while (array[i])
	{
		free(array[i]);
		i++;
	}
	free(array);
}

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
	char	*path_env;
	char	**path_dirs;

	if (!command || !*command)
		return (NULL);
	if (is_absolute_or_relative_path(command))
	{
		if (access(command, X_OK) == 0)
			return (ft_strdup(command));
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

int	setup_input_redirection(t_redirection *redir)
{
	int	fd;

	if (!redir)
		return (0);
	fd = open(redir->filename, O_RDONLY);
	if (fd == -1)
	{
		if (access(redir->filename, F_OK) == -1)
			ft_printf("minishell: %s: No such file or directory\n",
				redir->filename);
		else
			ft_printf("minishell: %s: Permission denied\n",
				redir->filename);
		return (-1);
	}
	if (dup2(fd, STDIN_FILENO) == -1)
	{
		close(fd);
		ft_printf("minishell: redirection failed\n");
		return (-1);
	}
	close(fd);
	return (0);
}

int	setup_output_redirection(t_redirection *redir)
{
	int	fd;

	if (!redir)
		return (0);
	fd = open(redir->filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
	{
		ft_printf("minishell: %s: Permission denied\n", redir->filename);
		return (-1);
	}
	if (dup2(fd, STDOUT_FILENO) == -1)
	{
		close(fd);
		ft_printf("minishell: redirection failed\n");
		return (-1);
	}
	close(fd);
	return (0);
}

static t_redirection	*get_last_redirection(t_redirection *redir_list)
{
	t_redirection	*current;

	if (!redir_list)
		return (NULL);
	current = redir_list;
	while (current->next)
		current = current->next;
	return (current);
}

static int	create_all_output_files(t_redirection *output_redirs)
{
	t_redirection	*current;
	int				fd;

	current = output_redirs;
	while (current)
	{
		fd = open(current->filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (fd == -1)
		{
			ft_printf("minishell: %s: Permission denied\n",
				current->filename);
			return (-1);
		}
		close(fd);
		current = current->next;
	}
	return (0);
}

int	setup_multiple_input_redirections(t_redirection *input_redirs)
{
	t_redirection	*last_input;

	if (!input_redirs)
		return (0);
	last_input = get_last_redirection(input_redirs);
	return (setup_input_redirection(last_input));
}

int	setup_multiple_output_redirections(t_redirection *output_redirs)
{
	t_redirection	*last_output;

	if (!output_redirs)
		return (0);
	if (create_all_output_files(output_redirs) == -1)
		return (-1);
	last_output = get_last_redirection(output_redirs);
	return (setup_output_redirection(last_output));
}
