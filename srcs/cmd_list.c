/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_list.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wchae <wchae@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/09 16:53:56 by wchae             #+#    #+#             */
/*   Updated: 2022/07/10 22:13:06 by wchae            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	cmd_lstadd_back(t_cmd **lst, t_token *tokens)
{
	t_cmd	*new;
	t_cmd	*phead;

	new = malloc(sizeof(t_cmd));
	new->tokens = tokens;
	new->redir = NULL;
	new->next = NULL;
	if (*lst == NULL)
	{
		*lst = new;
		return ;
	}
	phead = *lst;
	while (phead->next)
		phead = phead->next;
	phead->next = new;
}

void	del_cmd_list(t_cmd *cmd)
{
	t_cmd	*tmp;

	tmp = cmd;
	while (tmp)
	{
		tmp = cmd->next;
		del_env_lst(cmd->tokens);
		del_env_lst(cmd->redir);
		free(cmd);
		cmd = tmp;
	}
}

// TODO : expand_env $
t_cmd	*make_cmd_list(t_token *tokens)
{
	t_cmd	*cmd;
	t_token	*head;
	t_token	*prev;
	t_token	*cur;

	head = tokens;
	cur = tokens;
	cmd = NULL;
	while (cur)
	{
		prev = cur;
		cur = cur->next;
		if (ft_strncmp(prev->key, "|", -1) == 0)
		{
			free(prev->key);
			free(prev);
			prev = NULL;
		}
		if (cur && ft_strncmp(cur->key, "|", -1) == 0)
		{
			prev->next = NULL;
			cmd_lstadd_back(&cmd, head);
			head = cur->next;
		}
	}
	cmd_lstadd_back(&cmd, head);
	return (cmd);
}
