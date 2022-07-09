/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_execute.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wchae <wchae@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/09 17:30:16 by wchae             #+#    #+#             */
/*   Updated: 2022/07/09 17:37:18 by wchae            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	do_actual_path_cmd(t_cmd *cmd, char **args, char **envp)
{
	execve(cmd->tokens->key, args, envp);
	error_msg(strerror(errno));
	return (127);
}


int	do_cmd_child(t_env *env, t_cmd *cmd)
{
	char	**envp;
	char	**args;
	char	**path;
	int		i;

	envp = get_env_list(&env);
	args = tokens_to_strs(cmd->tokens);
	if (ft_strchr(cmd->tokens->key, '/'))
		return (do_actual_path_cmd(cmd, args, envp));
	if (find_env_node(env, "PATH"))
	{
		path = ft_split(find_env_node(env, "PATH")->value, ':');
		i = 0;
		while (path[i])
		{
			path[i] = ft_strjoin(path[i], "/");
			path[i] = ft_strjoin(path[i], args[0]);
			i++;
		}
		while (*path)
		{
			execve(*path, args, envp);
			if (errno != ENOENT)
				break ;
			path++;
		}
		if (errno != ENOENT)
		{
			error_msg(strerror(errno));
			return (126);
		}
	}
	error_msg("command not found");
	return (127);
}

int	do_cmd(t_env *env, t_cmd *cmd)
{
	pid_t	pid;
	char	**args;
	int		status;

	if (check_builtin_cmd(cmd->tokens))
	{
		args = tokens_to_strs(cmd->tokens);
		status = execute_builtin_cmd(env, cmd, args);
		ft_free_split(args);
		return (status);
	}
	else
	{
		pid = fork();
		if (pid == -1)
			exit(EXIT_FAILURE);
		else if (pid == 0)
			exit(do_cmd_child(env, cmd));
		waitpid(-1, &status, 0);
		if (WIFEXITED(status))
			return (WEXITSTATUS(status));
		return (WCOREFLAG | WTERMSIG(status));
	}
}

int	do_exec_function(t_env *env, t_token *tokens)
{
	t_cmd	*cmd;
	t_cmd	*tmp;
	int		n_cmd;

	cmd = make_cmd_list(env, tokens);
	set_redir(cmd);
	tmp = cmd;
	n_cmd = 0;
	while (tmp)
	{
		n_cmd++;
		tmp = tmp->next;
	}
	if (n_cmd == 1)
		g_status = do_cmd(env, cmd);
	else
		g_status = do_pipe(env, cmd, n_cmd - 1);
	del_cmd_list(cmd);
	return (g_status);
}
