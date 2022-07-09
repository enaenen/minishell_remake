/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_quote2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wchae <wchae@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/09 19:28:10 by wchae             #+#    #+#             */
/*   Updated: 2022/07/09 19:38:03 by wchae            ###   ########.fr       */
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

char	*skip_space(char *str)
{
	while (*str)
	{
		if (*str != ' ')
			break ;
		str++;
	}
	return (str);
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
			str = skip_space(str);
			ft_lstadd_back(&tmp_lst, ft_lstnew(put_str(buf)));
			if (*str == 0)
				break ;
		}
		str++;
	}
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