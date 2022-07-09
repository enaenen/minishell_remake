/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_quote.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wchae <wchae@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/09 16:45:43 by wchae             #+#    #+#             */
/*   Updated: 2022/07/09 18:01:15 by wchae            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_quote(char c)
{
	if (c == '\'')
		return (S_QUOTE);
	else if (c == '"')
		return (D_QUOTE);
	return (0);
}

char	*skip_quote(t_buffer *buf, char *data, int q_flag)
{
	data++;
	while (is_quote(*data) != q_flag)
	{
		add_char(buf, *data);
		data++;
	}
	return (data);
}

char	*skip_quote_2(t_buffer *buf, char *data, int q_flag)
{
	add_char(buf, *data);
	data++;
	while (is_quote(*data) != q_flag)
	{
		add_char(buf, *data);
		data++;
	}
	add_char(buf, *data);
	return (data);
}

char	*rm_quote(char *data)
{
	t_buffer	*buf;
	char		*str;

	buf = create_buf();
	while (*data)
	{
		if (is_quote(*data))
			data = skip_quote(buf, data, is_quote(*data));
		else
			add_char(buf, *data);
		data++;
	}
	str = put_str(buf);
	del_buf(buf);
	return (str);
}
