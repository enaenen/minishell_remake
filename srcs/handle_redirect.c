/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_redirect.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wchae <wchae@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/09 17:31:12 by wchae             #+#    #+#             */
/*   Updated: 2022/07/10 22:17:54 by wchae            ###   ########.fr       */
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

int	error_msg_ambiguous(char *str)
{
	t_buffer	*buf;
	char		*err_msg;

	buf = create_buf();
	add_str(buf, "minishell: ");
	add_str(buf, str);
	add_str(buf, ": ambiguous redirect\n");
	err_msg = put_str(buf);
	ft_putstr_fd(err_msg, STDERR_FILENO);
	del_buf(buf);
	free(err_msg);
	return (EXIT_FAILURE);
}

int	apply_redir(t_env *env, t_cmd *cmd)
{
	int		fd;
	t_redir	*redir;
	int		redir_flag;
	char	*expanded_str;
	char	**expanded_strs;
	int		len;

	redir = cmd->redir;
	while (redir)
	{
		redir_flag = is_redirection(redir->key);
		if (redir_flag && redir_flag != 2)
		{
			expanded_str = expand_str(env, redir->next->key);
			expanded_strs = split_skip_quote(expanded_str);
			free(expanded_str);
			len = 0;
			while (expanded_strs[len])
				len++;
			if (len == 0 || len > 1)
			{
				ft_free_split(expanded_strs);
				return (error_msg_ambiguous(redir->next->key));
			}
			free(redir->next->key);
			redir->next->key = rm_quote(expanded_strs[0]);
			ft_free_split(expanded_strs);
		}
		if (redir_flag == 1 || redir_flag == 2)
		{
			if (redir_flag == 1)
				fd = open(redir->next->key, O_RDONLY);
			else
				fd = expand_here_doc(env, redir);
			if (fd == -1)
				return (fd_print_err(redir->next->key));
			dup2(fd, STDIN_FILENO);
			close(fd);
		}
		else if (redir_flag == 3 || redir_flag == 4)
		{
			if (redir_flag == 3)
				fd = open(redir->next->key, O_TRUNC | O_CREAT | O_WRONLY, 0777);
			else
				fd = open(redir->next->key,
						O_APPEND | O_CREAT | O_WRONLY,
						0777);
			if (fd == -1)
				return (fd_print_err(redir->next->key));
			dup2(fd, STDOUT_FILENO);
			close(fd);
		}
		redir = redir->next->next;
	}
	return (EXIT_SUCCESS);
}
