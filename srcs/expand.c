/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wchae <wchae@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/09 17:25:48 by wchae             #+#    #+#             */
/*   Updated: 2022/07/09 17:32:09 by wchae            ###   ########.fr       */
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
	expanded_str = expand_data(env, redir->value);
	write(io_heredoc[1], expanded_str, ft_strlen(expanded_str));
	close(io_heredoc[1]);
	return (io_heredoc[0]);
}

char	*expand_data(t_env *env_list, char *data)
{
	t_buffer	*buf;
	char		*str;

	buf = create_buf();
	while (*data)
	{
		if (is_quote(*data) == S_QUOTE)
			data = skip_quote_2(buf, data, S_QUOTE);
		else if (*data == '$')
			data = replace_env_val(env_list, buf, data);
		else
			add_char(buf, *data);
		if (data == NULL)
			break ;
		data++;
	}
	str = put_str(buf);
	del_buf(buf);
	return (str);
}
