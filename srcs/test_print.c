/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_print.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wchae <wchae@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/09 16:29:16 by wchae             #+#    #+#             */
/*   Updated: 2022/07/09 16:55:16 by wchae            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_lstprint(t_list *lst)
{
	if (!lst)
		return ;
	while (lst)
	{
		ft_putstr(lst->data);
		ft_putstr("\n");
		lst = lst->next;
	}
}

void	print_env_list(t_env *env)
{
	while (env)
	{
		printf("key = %s ",env->key);
		env = env->next;
	}
	printf("\n");
}

void	print_cmd(t_cmd *cmd)
{
	while (cmd)
	{
		print_env_list(cmd->tokens);
		print_env_list(cmd->redir);
		cmd = cmd->next;
	}
}
