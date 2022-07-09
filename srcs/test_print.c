/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_print.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seseo <seseo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/09 16:29:16 by wchae             #+#    #+#             */
/*   Updated: 2022/07/10 02:07:07 by seseo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_lstprint(t_list *lst)
{
	if (!lst)
		return ;
	while (lst)
	{
		fprintf(stderr, "%s\n", lst->data);
		lst = lst->next;
	}
}

void	print_env_list(t_env *env)
{
	while (env)
	{
		fprintf(stderr, "key = %s ",env->key);
		env = env->next;
	}
	fprintf(stderr, "\n");
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

void	print_dchar(char **str)
{
	while (*str)
	{
		fprintf(stderr, "%s\n", *str);
		str++;
	}
	fprintf(stderr, "END\n");
}
