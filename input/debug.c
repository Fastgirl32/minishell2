#include "minishell.h"

char	*safe_text(char *s)
{
	if (s)
		return (s);
	return ("(null)");
}

void	print_one_command(int idx, t_command *head)
{
	ft_printf("[%d] cmd='%s' limiter='%s' ac=%d\n", idx,
		safe_text(head->command), safe_text(head->limiter), head->ac);
}

void	append_command(t_command **head, t_command **tail, t_command *new_cmd)
{
	if (!new_cmd)
		return ;
	if (!*head)
		*head = new_cmd;
	else
		(*tail)->next = new_cmd;
	*tail = new_cmd;
}

void	print_argv_debug(char **argv)
{
	int	i;

	i = 0;
	while (argv && argv[i])
	{
		ft_printf("    argv[%d]='%s'\n", i, argv[i]);
		i++;
	}
}

void	print_command_list(t_command *head)
{
	int	idx;

	ft_printf("\n--- command list ---\n");
	if (!head)
		return ((void)ft_printf("(empty)\n"));
	idx = 0;
	while (head)
	{
		print_one_command(idx, head);
		print_argv_debug(head->argv);
		head = head->next;
		idx++;
	}
	ft_printf("--------------------\n");
}
