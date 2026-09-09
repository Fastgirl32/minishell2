/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saecker <saecker@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/01 17:18:22 by lstarek           #+#    #+#             */
/*   Updated: 2026/09/10 01:33:30 by saecker          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/* Returns text or a visible placeholder for a null string. */
char	*safe_text(char *s)
{
	if (s)
		return (s);
	return ("(null)");
}

/* Prints one command in debug mode. */
void	print_one_command(int idx, t_command *head)
{
	ft_printf("[%d] cmd='%s' limiter='%s' ac=%d\n", idx,
		safe_text(head->command), safe_text(head->limiter), head->ac);
	ft_printf("fd_in: %d. fd_out: %d\n", head->fd_in, head->fd_out);
}

/* Appends a command to a linked command list. */
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

/* Prints command arguments in debug mode. */
void print_argv_debug(char **argv)
{
    int i;

    i = 0;
    while (argv && argv[i])
    {
        ft_printf("    argv[%d]='%s'\n", i, argv[i]);
        i++;
    }
}

/* Prints the complete command list in debug mode. */
void	print_command_list(t_command *head)
{
	int	idx;

	if (!VERBOSE)
		return ;
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
