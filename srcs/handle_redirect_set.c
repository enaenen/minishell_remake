/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_redirect_set.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seseo <seseo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/09 17:31:12 by wchae             #+#    #+#             */
/*   Updated: 2022/07/11 11:13:02 by seseo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	set_redir_from_cur_tokens(t_cmd *cmd_tmp, t_token *cur);

void	set_redir(t_cmd *cmd)
{
	t_cmd	*cmd_tmp;
	t_token	*cur;

	cmd_tmp = cmd;
	while (cmd_tmp)
	{
		cur = cmd_tmp->tokens;
		cmd_tmp->tokens = NULL;
		set_redir_from_cur_tokens(cmd_tmp, cur);
		cmd_tmp = cmd_tmp->next;
	}
}

static void	set_redir_from_cur_tokens(t_cmd *cmd_tmp, t_token *cur)
{
	t_token	*head;
	t_token	*prev;

	head = NULL;
	while (cur)
	{
		if (is_redirection(cur->key))
		{
			head = cur;
			cur = head->next->next;
			head->next->next = NULL;
			env_lstadd_back_node(&cmd_tmp->redir, head);
			continue ;
		}
		prev = cur;
		cur = cur->next;
		prev->next = NULL;
		env_lstadd_back_node(&cmd_tmp->tokens, prev);
	}
}

int	is_redirection(char *str)
{
	if (ft_strcmp(str, "<") == 0)
		return (1);
	else if (ft_strcmp(str, "<<") == 0)
		return (2);
	else if (ft_strcmp(str, ">") == 0)
		return (3);
	else if (ft_strcmp(str, ">>") == 0)
		return (4);
	else
		return (0);
}
