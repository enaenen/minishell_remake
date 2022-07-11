/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wchae <wchae@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/09 17:30:16 by wchae             #+#    #+#             */
/*   Updated: 2022/07/11 00:43:31 by wchae            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	do_exec_function(t_env *env, t_token *tokens)
{
	t_cmd	*cmd;
	t_cmd	*tmp;
	int		n_cmd;
	int		status;

	cmd = make_cmd_list(tokens);
	set_redir(cmd);
	tmp = cmd;
	n_cmd = 0;
	while (tmp)
	{
		n_cmd++;
		tmp = tmp->next;
	}
	if (n_cmd == 1)
		status = do_cmd(env, cmd);
	else
		status = do_pipe(env, cmd, n_cmd - 1);
	del_cmd_list(cmd);
	return (status);
}
