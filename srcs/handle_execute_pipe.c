/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_execute_pipe.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seseo <seseo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/09 17:35:59 by wchae             #+#    #+#             */
/*   Updated: 2022/07/10 04:23:49 by seseo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	do_final_pipe_cmd(t_env *env, t_cmd *cmd, int n_pipe, int prev_fd)
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
			r_status = status;
		i++;
	}
	if (WIFEXITED(r_status))
		return (WEXITSTATUS(r_status));
	return (WCOREFLAG | WTERMSIG(r_status));
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

char	*expand_str(t_env *env, char *str)
{
	char		*expanded_str;
	t_buffer	*buf;

	buf = create_buf();
	while (*str)
	{
		if (is_quote(*str) == S_QUOTE)
			str = skip_quote_2(buf, str, is_quote(*str));
		else if (*str == '$')
			str = replace_env_val(env, buf, str);
		else
			add_char(buf, *str);
		str++;
	}
	expanded_str = put_str(buf);
	del_buf(buf);
	return (expanded_str);
}

void	expand_tokens(t_env *env, t_token *tokens)
{
	t_token		*tmp_next;
	char		**strs;
	char		*str;
	int			i;

	while (tokens)
	{
		tmp_next = tokens->next;
		tokens->next = NULL;
		str = expand_str(env, tokens->key);
		strs = split_skip_quote(str);
		free(str);
		free(tokens->key);
		tokens->key = strs[0];
		i = 1;
		while (strs[0] && strs[i])
			env_lstadd_back(&tokens, strs[i++], NULL);
		free(strs);
		while (tokens->next)
			tokens = tokens->next;
		tokens->next = tmp_next;
		tokens = tmp_next;
	}
}

// void	expand_redir(t_env *env, t_redir *redir)
// {
// 	t_token		*tmp_next;
// 	char		**strs;
// 	char		*str;
// 	int			i;

// 	while (redir)
// 	{
// 		if (ft_strncmp(redir->key, "<<", -1))
// 		{
// 			tmp_next = redir->next->next;
// 			redir->next->next = NULL;
// 			str = expand_str(env, redir->next->key);
// 			strs = split_skip_quote(str);
// 			free(str);
// 			free(tmp->key);
// 			tmp->key = strs[0];
// 			i = 1;
// 			while (strs[i])
// 				env_lstadd_back(&tmp, strs[i++], NULL);
// 			while (tmp->next)
// 				tmp = tmp->next;
// 			tmp->next = tmp_next;
// 		}
// 		else
// 			redir = redir->next->next;
// 	}
// }

int	do_pipe_cmd(t_env *env, t_cmd *cmd)
{
	expand_tokens(env, cmd->tokens);
	rm_quote_tokens(cmd->tokens);
	if (!cmd->tokens->key)
		return (EXIT_SUCCESS);
	if (check_builtin_cmd(cmd->tokens))
		return (execute_builtin_cmd_pipe(env, cmd, tokens_to_strs(cmd->tokens)));
	if (ft_strchr(cmd->tokens->key, '/'))
		return (do_actual_path_cmd(cmd,
				tokens_to_strs(cmd->tokens),
				get_env_list(&env)));
	return (do_cmd_child(env, cmd));
}
