#include "../minishell.h"

/*
Frees a double pointer passed as (void **)
*/
void	free_arr(void **arr)
{
	int		i;

	i = 0;
	if (!arr)
		return ;
	while (arr[i])
	{
		free(arr[i]);
		i++;
	}
	free(arr);
}

/*
Frees the vars struct properly.
*/
void	free_vars(t_vars *vars)
{
	if (!vars)
		return ;
	free_arr((void **)(vars->history));
    free_arr((void **)(vars->env));
    free(vars);
}