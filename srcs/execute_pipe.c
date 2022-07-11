/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipe.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seseo <seseo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/09 17:35:59 by wchae             #+#    #+#             */
/*   Updated: 2022/07/11 11:25:14 by seseo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	do_pipe_child(t_env *env, t_cmd *cmd, int pipe_fd[2], int prev_fd);
static int	do_pipe_cmd(t_env *env, t_cmd *cmd);
static int	do_final_pipe_cmd(t_env *env, t_cmd *cmd, int n_pipe, int prev_fd);
static void	do_final_pipe_child(t_env *env, t_cmd *cmd, int prev_fd);

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
			ft_kill_exit();
		pid = fork();
		if (pid == -1)
			ft_kill_exit();
		else if (pid == 0)
			do_pipe_child(env, cmd, pipe_fd, prev_fd);
		if (prev_fd != -1)
			close(prev_fd);
		close(pipe_fd[1]);
		prev_fd = pipe_fd[0];
		cmd = cmd->next;
		i++;
	}
	return (do_final_pipe_cmd(env, cmd, n_pipe, prev_fd));
}

static void	do_pipe_child(t_env *env, t_cmd *cmd, int pipe_fd[2], int prev_fd)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	if (prev_fd != -1)
	{
		dup2(prev_fd, STDIN_FILENO);
		close(prev_fd);
	}
	close(pipe_fd[0]);
	dup2(pipe_fd[1], STDOUT_FILENO);
	close(pipe_fd[1]);
	if (apply_redir(env, cmd))
		exit(EXIT_FAILURE);
	exit(do_pipe_cmd(env, cmd));
}

static int	do_pipe_cmd(t_env *env, t_cmd *cmd)
{
	expand_tokens(env, cmd->tokens);
	rm_quote_tokens(cmd->tokens);
	if (!cmd->tokens || !cmd->tokens->key)
		return (EXIT_SUCCESS);
	if (check_builtin_cmd(cmd->tokens))
		return (execute_builtin_cmd_pipe(
				env, cmd, tokens_to_strs(cmd->tokens)));
	if (ft_strchr(cmd->tokens->key, '/'))
		return (do_actual_path_cmd(cmd,
				tokens_to_strs(cmd->tokens),
				get_env_list(&env)));
	return (do_cmd_child(env, cmd));
}

static int	do_final_pipe_cmd(t_env *env, t_cmd *cmd, int n_pipe, int prev_fd)
{
	pid_t	pid;
	int		status;
	int		r_status;
	int		i;

	pid = fork();
	if (pid == -1)
	{
		kill(0, SIGKILL);
		exit(EXIT_FAILURE);
	}
	else if (pid == 0)
		do_final_pipe_child(env, cmd, prev_fd);
	close(prev_fd);
	i = 0;
	while (i < n_pipe + 1)
	{
		if (waitpid(-1, &status, 0) == pid)
			r_status = status;
		i++;
	}
	if (WIFEXITED(r_status))
		return (WEXITSTATUS(r_status));
	return (WCOREFLAG | WTERMSIG(r_status));
}

static void	do_final_pipe_child(t_env *env, t_cmd *cmd, int prev_fd)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	dup2(prev_fd, STDIN_FILENO);
	close(prev_fd);
	if (apply_redir(env, cmd))
		exit(EXIT_FAILURE);
	exit(do_pipe_cmd(env, cmd));
}
