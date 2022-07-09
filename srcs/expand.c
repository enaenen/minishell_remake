/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seseo <seseo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/09 17:25:48 by wchae             #+#    #+#             */
/*   Updated: 2022/07/10 04:34:51 by seseo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	expand_here_doc(t_env *env, t_redir *redir)
{
	t_buffer	*buf;
	char		*expanded_str;
	int			io_heredoc[2];

	if (pipe(io_heredoc))
		exit(EXIT_FAILURE);
	buf = create_buf();
	expanded_str = expand_str(env, redir->value);
	write(io_heredoc[1], expanded_str, ft_strlen(expanded_str));
	close(io_heredoc[1]);
	return (io_heredoc[0]);
}
