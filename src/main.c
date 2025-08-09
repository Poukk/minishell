/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexanfe <alexanfe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 23:29:41 by alexanfe          #+#    #+#             */
/*   Updated: 2025/06/16 23:29:43 by alexanfe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"


char	**cell_split_line(char *line)
{
	char	**tokens;

	tokens = ft_split(line, ' ');
	if (!tokens)
	{
		perror("ft_split failed");
		exit(EXIT_FAILURE);
	}
	return (tokens);
}

char	*cell_read_line(void)
{
	char	*buf;
	char	cwd[BUFSIZ];

	get_cwd(cwd, sizeof(cwd));
	ft_printf("%s$>", cwd);

	buf = get_next_line(0); //stdin = 0
	if (!buf)
	{
		if (feof(stdin))
			ft_printf("End of File!");
		else
			ft_printf("get_next_line failed!");
	}
	return (buf);
}

int	main(int ac, char **av) 
{
	char	*line;
	char	**args;
	int		i;
	(void)ac;
	(void)av;

	while(1)
	{
		//prompt + get line
		line = cell_read_line();
		if (!line)
			break;
		//get tokens gettok (->lexing->parsing = EVALUATING)
		args = cell_split_line(line);
		i = 0;
		while (args[i])
		{
			ft_printf("%s\n", args[i]);
			i++;
		}

		//exec


		free_tokens(args);
		free(line);
	}

	return (EXIT_SUCCESS);
}
