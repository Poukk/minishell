#include "minishell.h"
#include "libft.h"

int	main(void)
{
	int a, b;
	a = 5;
	b = 10;
	ft_printf("Sum of %d and %d is %d\n", a, b, sum(a, b));
	return (0);
}
