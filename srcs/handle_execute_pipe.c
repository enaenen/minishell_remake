/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_execute_pipe.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wchae <wchae@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/09 17:35:59 by wchae             #+#    #+#             */
/*   Updated: 2022/07/09 17:36:26 by wchae            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	do_final_pipe_cmd(t_env *env, t_cmd *cmd, int n_pipe, int prev_fd)
{
	pid_t	pid;
	int		status;
	int		i;

	pid = fork();
	if (pid == -1)
	{
		kill(0, SIGKILL);
		exit(EXIT_FAILURE);
	}
	else if (pid == 0)
	{
		dup2(prev_fd, STDIN_FILENO);
		close(prev_fd);
		apply_redir(env, cmd);
		exit(do_pipe_cmd(env, cmd));
	}
	close(prev_fd);
	i = 0;
	while (i < n_pipe + 1)
	{
		if (waitpid(-1, &status, 0) == pid)
			g_status = status;
		i++;
	}
	return (g_status);
}

int	do_pipe(t_env *env, t_cmd *cmd, int n_pipe)
{
	int		i;
	pid_t	pid;
	int		prev_fd;
	int		pipe_fd[2];

	prev_fd = -1;
	i = 0;
	while (i < n_pipe)
	{
		if (pipe(pipe_fd))
		{
			kill(0, SIGKILL);
			exit(EXIT_FAILURE);
		}
		pid = fork();
		if (pid == -1)
		{
			kill(0, SIGKILL);
			exit(EXIT_FAILURE);
		}
		else if (pid == 0)
		{
			if (prev_fd != -1)
			{
				dup2(prev_fd, STDIN_FILENO);
				close(prev_fd);
			}
			close(pipe_fd[0]);
			dup2(pipe_fd[1], STDOUT_FILENO);
			close(pipe_fd[1]);
			apply_redir(env, cmd);
			exit(do_pipe_cmd(env, cmd));
		}
		if (prev_fd != -1)
			close(prev_fd);
		close(pipe_fd[1]);
		prev_fd = pipe_fd[0];
		cmd = cmd->next;
		i++;
	}
	return (do_final_pipe_cmd(env, cmd, n_pipe, prev_fd));
}

int	do_pipe_cmd(t_env *env, t_cmd *cmd)
{

	if (check_builtin_cmd(cmd->tokens))
		return (execute_builtin_cmd(env, cmd, tokens_to_strs(cmd->tokens)));
	if (ft_strchr(cmd->tokens->key, '/'))
		return (do_actual_path_cmd(cmd,
				tokens_to_strs(cmd->tokens),
				get_env_list(&env)));
	return (do_cmd_child(env, cmd));
}
