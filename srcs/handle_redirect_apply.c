/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_redirect_apply.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seseo <seseo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/09 17:31:12 by wchae             #+#    #+#             */
/*   Updated: 2022/07/11 11:17:23 by seseo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	set_input_redir(t_env *env, t_redir *redir, int redir_flag);
static int	set_output_redir(t_redir *redir, int redir_flag);
static int	redirection_expand(t_env *env, t_redir *redir, int redir_flag);

int	apply_redir(t_env *env, t_cmd *cmd)
{
	t_redir	*redir;
	int		redir_flag;

	redir = cmd->redir;
	while (redir)
	{
		redir_flag = is_redirection(redir->key);
		if (redirection_expand(env, redir, redir_flag))
			return (EXIT_FAILURE);
		if (set_input_redir(env, redir, redir_flag))
			return (EXIT_FAILURE);
		if (set_output_redir(redir, redir_flag))
			return (EXIT_FAILURE);
		redir = redir->next->next;
	}
	return (EXIT_SUCCESS);
}

static int	redirection_expand(t_env *env, t_redir *redir, int redir_flag)
{
	char	*expanded_str;
	char	**expanded_strs;
	int		len;

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
	return (EXIT_SUCCESS);
}

static int	set_input_redir(t_env *env, t_redir *redir, int redir_flag)
{
	int		fd;

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
	return (EXIT_SUCCESS);
}

static int	set_output_redir(t_redir *redir, int redir_flag)
{
	int		fd;

	if (redir_flag == 3 || redir_flag == 4)
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
	return (EXIT_SUCCESS);
}
