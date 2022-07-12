/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_quote_2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seseo <seseo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/09 19:28:10 by wchae             #+#    #+#             */
/*   Updated: 2022/07/12 15:16:42 by seseo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	find_valid_quot_point(char *data, int start)
{
	int	find;

	find = start + 1;
	while (data[find] && data[start] != data[find])
		find++;
	if (data[find])
		return (find);
	return (start);
}

char	**split_skip_quote(char *str)
{
	t_buffer	*buf;
	t_list		*tmp_lst;
	char		**strs;

	tmp_lst = NULL;
	buf = create_buf();
	while (*str)
	{
		if (is_quote(*str))
			str = skip_quote_2(buf, str, is_quote(*str));
		else if (*str != ' ')
			add_char(buf, *str);
		else
		{
			if (buf->len != 0)
				ft_lstadd_back(&tmp_lst, ft_lstnew(put_str(buf)));
		}
		str++;
	}
	if (buf->len)
		ft_lstadd_back(&tmp_lst, ft_lstnew(put_str(buf)));
	strs = lst_to_strs(tmp_lst);
	ft_lstclear(&tmp_lst, free);
	del_buf(buf);
	return (strs);
}

char	**split_cmd(t_list *cmd)
{
	char	**exe;
	int		size;
	int		i;

	size = ft_lstsize(cmd);
	exe = (char **)malloc((size + 1) * sizeof(char *));
	if (!exe)
		return (NULL);
	i = 0;
	while (i < size)
	{
		exe[i] = cmd->data;
		cmd = cmd->next;
		i++;
	}
	exe[i] = NULL;
	return (exe);
}

void	rm_quote_tokens(t_token *tokens)
{
	char	*tmp;

	while (tokens)
	{
		tmp = tokens->key;
		if (tmp)
			tokens->key = rm_quote(tmp);
		free(tmp);
		tokens = tokens->next;
	}
}
