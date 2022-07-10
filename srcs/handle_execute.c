/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_execute.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seseo <seseo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/09 17:30:16 by wchae             #+#    #+#             */
/*   Updated: 2022/07/10 18:21:29 by seseo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	do_actual_path_cmd(t_cmd *cmd, char **args, char **envp)
{
	execve(cmd->tokens->key, args, envp);
	if (errno != ENOENT)
		return (error_msg(args[0]));
	return (error_msg_cmd_not_found(args[0]));
}

int	do_cmd_child(t_env *env, t_cmd *cmd)
{
	char	**envp;
	char	**args;
	char	**path;
	int		i;

	if (!cmd->tokens || !cmd->tokens->key)
		return (EXIT_SUCCESS);
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
			return (error_msg(args[0]));
	}
	return (error_msg_cmd_not_found(args[0]));
}

void	backup_fd(int backup_io[2])
{
	backup_io[0] = dup(STDIN_FILENO);
	backup_io[1] = dup(STDOUT_FILENO);
}

void	restore_fd(int backup_io[2])
{
	int	rd_io[2];

	rd_io[0] = dup(STDIN_FILENO);
	rd_io[1] = dup(STDOUT_FILENO);
	dup2(backup_io[0], STDIN_FILENO);
	dup2(backup_io[1], STDOUT_FILENO);
	close(backup_io[0]);
	close(backup_io[1]);
	close(rd_io[0]);
	close(rd_io[1]);
}

int	do_cmd(t_env *env, t_cmd *cmd)
{
	pid_t	pid;
	char	**args;
	int		status;
	int		backup_io[2];

	expand_tokens(env, cmd->tokens);
	rm_quote_tokens(cmd->tokens);
	if (cmd->tokens && check_builtin_cmd(cmd->tokens))
	{
		backup_fd(backup_io);
		if (apply_redir(env, cmd))
			return (EXIT_FAILURE);
		args = tokens_to_strs(cmd->tokens);
		status = execute_builtin_cmd(env, cmd, args);
		ft_free_split(args);
		restore_fd(backup_io);
		return (status);
	}
	pid = fork();
	if (pid == -1)
		exit(EXIT_FAILURE);
	else if (pid == 0)
	{
		if (apply_redir(env, cmd))
			exit(EXIT_FAILURE);
		exit(do_cmd_child(env, cmd));
	}
	waitpid(-1, &status, 0);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	return (WCOREFLAG | WTERMSIG(status));
}

int	do_exec_function(t_env *env, t_token *tokens)
{
	t_cmd	*cmd;
	t_cmd	*tmp;
	int		n_cmd;
	int		status;

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
		status = do_cmd(env, cmd);
	else
		status = do_pipe(env, cmd, n_cmd - 1);
	del_cmd_list(cmd);
	return (status);
}
