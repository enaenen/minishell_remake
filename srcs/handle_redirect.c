/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_redirect.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wchae <wchae@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/09 17:31:12 by wchae             #+#    #+#             */
/*   Updated: 2022/07/09 17:31:52 by wchae            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


void	set_redir(t_cmd *cmd)
{
	t_cmd	*cmd_tmp;
	t_token	*head;
	t_token	*prev;
	t_token	*cur;

	cmd_tmp = cmd;
	while (cmd_tmp)
	{
		cur = cmd_tmp->tokens;
		cmd_tmp->tokens = NULL;
		head = NULL;
		while (cur)
		{
			if (cur && (ft_strncmp(cur->key, "<", 1) == 0
					|| ft_strncmp(cur->key, ">", 1) == 0))
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
		cmd_tmp = cmd_tmp->next;
	}
}


int	apply_redir(t_env *env, t_cmd *cmd)
{
	int		fd;
	t_redir	*redir;

	redir = cmd->redir;
	while (redir)
	{
		if (ft_strncmp(redir->key, "<", -1) == 0
			|| ft_strncmp(redir->key, "<<", -1) == 0)
		{
			if (ft_strncmp(redir->key, "<", -1) == 0)
				fd = open(redir->next->key, O_RDONLY);
			else
				fd = expand_here_doc(env, redir);
			if (fd_print_err(fd))
				return (EXIT_FAILURE);
			dup2(fd, STDIN_FILENO);
			close(fd);
		}
		else
		{
			if (ft_strncmp(redir->key, ">", -1) == 0)
				fd = open(redir->next->key, O_TRUNC | O_CREAT | O_WRONLY, 0777);
			else
				fd = open(redir->next->key,
						O_APPEND | O_CREAT | O_WRONLY,
						0777);
			if (fd_print_err(fd))
				return (EXIT_FAILURE);
			dup2(fd, STDOUT_FILENO);
			close(fd);
		}
		redir = redir->next->next;
	}
	return (EXIT_SUCCESS);
}
