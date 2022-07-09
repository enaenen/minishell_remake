/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_env.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wchae <wchae@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/04 02:25:03 by wchae             #+#    #+#             */
/*   Updated: 2022/07/09 19:15:38 by wchae            ###   ########.fr       */
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
		str = ft_itoa(g_status);
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
			key = ft_substr(data, 1, i);
			str = read_key(env, key);
			free(key);
		}
	}
	add_str(buf, str);
	free(str);
	return (data);
}

static void	get_env_convert(t_env **env_list, char **env)
{
	t_env		*tmp;
	t_buffer	*buf;
	int			i;

	buf = create_buf();
	tmp = *env_list;
	i = 0;
	while (tmp)
	{
		if (tmp->value)
		{
			add_str(buf, tmp->key);
			add_char(buf, '=');
			add_str(buf, tmp->value);
			env[i++] = put_str(buf);
		}
		tmp = tmp->next;
	}
	del_buf(buf);
}

char	**get_env_list(t_env **env_list)
{
	t_env	*tmp;
	char	**env;
	int		i;

	i = 0;
	tmp = *env_list;
	while (tmp)
	{
		if (tmp->value)
			i++;
		tmp = tmp->next;
	}
	env = malloc(sizeof(char *) * (i + 1));
	if (env == NULL)
		exit(1);
	env[i] = NULL;
	get_env_convert(env_list, env);
	return (env);
}

char	*read_key(t_env *env_list, char *key)
{
	while (env_list)
	{
		if (!ft_strcmp(env_list->key, key))
			return (ft_strdup(env_list->value));
		env_list = env_list->next;
	}
	return (NULL);
}

t_env	*find_env_node(t_env *env_list, char *key)
{
	while (env_list)
	{
		if (ft_strncmp(env_list->key, key, -1) == 0)
			return (env_list);
		env_list = env_list->next;
	}
	return (NULL);
}
