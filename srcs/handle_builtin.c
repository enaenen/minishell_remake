/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_builtin.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wchae <wchae@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/28 00:05:09 by wchae             #+#    #+#             */
/*   Updated: 2022/07/09 16:54:02 by wchae            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

int	execute_builtin_cmd(t_env *env_list, t_cmd *cmd, char **exe)
{
	if (ft_strncmp(cmd->tokens->key, "echo", 5) == 0)
		return (ft_echo(&exe[1]));
	else if (ft_strncmp(cmd->tokens->key, "cd", 3) == 0)
		return (ft_cd(&exe[1], env_list));
	else if (ft_strncmp(cmd->tokens->key, "pwd", 4) == 0)
		return (ft_pwd());
	else if (ft_strncmp(cmd->tokens->key, "export", 7) == 0)
		return (ft_export(&exe[1], env_list, 0, 0));
	else if (ft_strncmp(cmd->tokens->key, "unset", 6) == 0)
		return (ft_unset(&exe[1], &env_list));
	else if (ft_strncmp(cmd->tokens->key, "env", 4) == 0)
		return (ft_env(env_list));
	else
		return (ft_exit(&exe[1]));
}
