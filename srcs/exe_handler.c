/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe_handler.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seseo <seseo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/28 00:05:09 by wchae             #+#    #+#             */
/*   Updated: 2022/07/08 22:39:59 by seseo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	check_builtin_cmd(t_token *tokens)
{
	if (ft_strncmp(tokens->key, "echo", 5) == 0
		|| ft_strncmp(tokens->key, "cd", 3) == 0
		|| ft_strncmp(tokens->key, "pwd", 4) == 0
		|| ft_strncmp(tokens->key, "export", 7) == 0
		|| ft_strncmp(tokens->key, "unset", 6) == 0
		|| ft_strncmp(tokens->key, "env", 4) == 0
		|| ft_strncmp(tokens->key, "exit", 5) == 0)
		return (TRUE);
	return (FALSE);
}

// void	execute_builtin_cmd(t_proc *proc, char **exe)
// {
// 		if (ft_strncmp(proc->tokens->key, "echo", 5) == 0)
// 			ft_echo(&exe[1]);
// 		if (ft_strncmp(proc->tokens->key, "cd", 3) == 0)
// 			// ft_cd2(proc, &exe[1]);
// 			ft_cd(&exe[1], proc->env_list);
// 		if (ft_strncmp(proc->tokens->key, "pwd", 4) == 0)
// 			ft_pwd();
// 		if (ft_strncmp(proc->tokens->key, "export", 7) == 0)
// 			ft_export(&exe[1], proc->env_list, 0, 0);
// 		if (ft_strncmp(proc->tokens->key, "unset", 6) == 0)
// 			ft_unset(&exe[1], &proc->env_list);
// 		if (ft_strncmp(proc->tokens->key, "env", 4) == 0)
// 			ft_env(proc->env_list);
// 		if (ft_strncmp(proc->tokens->key, "exit", 5) == 0)
// 			ft_exit(&exe[1]);
// 		free(exe);
// }
