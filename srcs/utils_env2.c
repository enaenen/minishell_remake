/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_env2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wchae <wchae@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/04 02:25:03 by wchae             #+#    #+#             */
/*   Updated: 2022/07/11 01:33:55 by wchae            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*replace_env_val(t_env *env, t_buffer *buf, char *data)
{
	char	*key;
	char	*str;
	int		i;

	i = 1;
	if (*(data + 1) == '?')
	{
		data++;
		str = ft_itoa(g_status);
	}
	else if (*(data + 1) == 0)
		str = ft_strdup("$");
	else if (ft_isdigit(*(data + 1)))
		str = NULL;
	else
	{
		while (data[i] && (ft_isalnum(data[i]) || data[i] == '_'))
			i++;
		if (i == 1)
			str = ft_substr(data, 0, 2);
		else
		{
			key = ft_substr(data, 1, i - 1);
			if (find_env_node(env, key))
				str = read_key(env, key);
			else
				str = NULL;
			free(key);
		}
		data += (i - 1);
	}
	add_str(buf, str);
	free(str);
	return (data);
}
