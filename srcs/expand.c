/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wchae <wchae@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/09 17:25:48 by wchae             #+#    #+#             */
/*   Updated: 2022/07/11 12:55:55 by wchae            ###   ########.fr       */
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
	if (ft_strcmp(redir->next->key, redir->next->value) == 0)
		expanded_str = expand_str(env, redir->value);
	else
		expanded_str = ft_strdup(redir->value);
	write(io_heredoc[1], expanded_str, ft_strlen(expanded_str));
	close(io_heredoc[1]);
	return (io_heredoc[0]);
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
