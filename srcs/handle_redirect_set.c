/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_redirect_set.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wchae <wchae@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/09 17:31:12 by wchae             #+#    #+#             */
/*   Updated: 2022/07/11 22:20:03 by wchae            ###   ########.fr       */
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

void	test_printcmd(t_cmd *cmd)
{
	t_cmd *pcmd;

	pcmd = cmd;
	while (pcmd)
	{
		printf("Tk = %s Tv = %s \n",pcmd->tokens->key, pcmd->tokens->value);
		// printf("Rk = %s Rv = %s \n",pcmd->redir->key, pcmd->redir->value);
		pcmd = pcmd->next;
	}
}
void 	testprint(t_token *tk)
{
	t_token	*map;

	map = tk;
	while (map)
	{
		printf("key : %s value : %s\n", map->key, map->value);
		map = map->next;
	}
}
static void	set_redir_from_cur_tokens(t_cmd *cmd_tmp, t_token *cur)
{
	t_token	*head;
	t_token	*prev;

	head = NULL;
	while (cur)
	{
		testprint(cur);
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
