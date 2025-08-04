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

char	*cell_read_line(void)
{
	char	*buf;
	size_t	bufsize;
	char	cwd[BUFSIZ];

	buf = NULL;

	ft_printf("minishel>:");
	if (getline(&buf, &bufsize, stdin) == -1)
	{
		buf = NULL;
		if (feof(stdin))
			ft_printf("End of File!");
		else
			ft_printf("Getline failed");
		ft_printf("%s\n", buf);
	}
	return (buf);
}

int	main(int ac, char **av) 
{
	char	*line;
	(void)ac;
	(void)av;
	//READ->EVALUATE->PRINT/EXECUTE->LOOP
	while(1)
	{
		//get line
		line = cell_read_line();
		ft_printf("%s\n", line);
		pause();

		//get tokens gettok (->lexing->parsing = EVALUATING)

		//exec
	}

	return (EXIT_SUCCESS);
}
