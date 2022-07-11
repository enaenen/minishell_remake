/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_env2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seseo <seseo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/04 02:25:03 by wchae             #+#    #+#             */
/*   Updated: 2022/07/11 12:54:10 by seseo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*find_key_and_get_val_str(t_env *env, char **str, char *data);

char	*replace_env_val(t_env *env, t_buffer *buf, char *data)
{
	char	*str;

	if (*(data + 1) == '?')
	{
		data++;
		str = ft_itoa(g_status);
	}
	else if (*(data + 1) == 0)
		str = ft_strdup("$");
	else if (ft_isdigit(*(data + 1)))
	{
		data++;
		str = NULL;
	}
	else
		data = find_key_and_get_val_str(env, &str, data);
	add_str(buf, str);
	free(str);
	return (data);
}

static char	*find_key_and_get_val_str(t_env *env, char **str, char *data)
{
	char	*key;
	int		i;

	i = 1;
	while (data[i] && (ft_isalnum(data[i]) || data[i] == '_'))
		i++;
	if (i == 1)
		*str = ft_substr(data, 0, 2);
	else
	{
		key = ft_substr(data, 1, i - 1);
		if (find_env_node(env, key))
			*str = read_key(env, key);
		else
			*str = NULL;
		free(key);
	}
	data += (i - 1);
	return (data);
}
