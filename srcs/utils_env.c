/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_env.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wchae <wchae@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/04 02:25:03 by wchae             #+#    #+#             */
/*   Updated: 2022/07/09 19:25:48 by wchae            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_env	*env_dup_check(t_env *env_list, char *new_key)
{
	t_env	*temp;

	temp = env_list;
	while (temp)
	{
		if (!ft_strcmp(temp->key, new_key))
			return (temp);
		temp = temp->next;
	}
	return (NULL);
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
