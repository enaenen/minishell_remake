/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe_handler.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wchae <wchae@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/28 00:05:09 by wchae             #+#    #+#             */
/*   Updated: 2022/07/04 19:57:18 by wchae            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	check_builtin_cmd(t_list *cmd)
{
	if (ft_strncmp(cmd->data, "echo", 5) == 0
		|| ft_strncmp(cmd->data, "cd", 3) == 0
		|| ft_strncmp(cmd->data, "pwd", 4) == 0
		|| ft_strncmp(cmd->data, "export", 7) == 0
		|| ft_strncmp(cmd->data, "unset", 6) == 0
		|| ft_strncmp(cmd->data, "env", 4) == 0
		|| ft_strncmp(cmd->data, "exit", 5) == 0)
		return (TRUE);
	cmd = cmd->next;
	return (FALSE);
}

void	execute_builtin_cmd(t_proc *proc, char **exe)
{
		if (ft_strncmp(proc->cmd->data, "echo", 5) == 0)
			ft_echo(&exe[1]);
		if (ft_strncmp(proc->cmd->data, "cd", 3) == 0)
			// ft_cd2(proc, &exe[1]);
			ft_cd(&exe[1], proc->env_list);
		if (ft_strncmp(proc->cmd->data, "pwd", 4) == 0)
			ft_pwd();
		if (ft_strncmp(proc->cmd->data, "export", 7) == 0)
			ft_export(&exe[1], proc->env_list, 0, 0);
		if (ft_strncmp(proc->cmd->data, "unset", 6) == 0)
			ft_unset(&exe[1], &proc->env_list);
		if (ft_strncmp(proc->cmd->data, "env", 4) == 0)
			ft_env(proc->env_list);
		if (ft_strncmp(proc->cmd->data, "exit", 5) == 0)
			ft_exit(&exe[1]);
		free(exe);
}
