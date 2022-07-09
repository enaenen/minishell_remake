/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wchae <wchae@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/04 18:43:23 by wchae             #+#    #+#             */
/*   Updated: 2022/07/09 23:00:47 by wchae            ###   ########.fr       */
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

int	export_key_syntax_error(char *s)
{
	write(2, "bash: export: `", 15);
	write(2, s, ft_strlen(s));
	write(2, "': not a valid identifier\n", 27);
	g_status = 1;
	return (0);
}

int	export_key_syntax_check(char *s)
{
	int	i;

	i = -1;
	if (s[0] > 47 && s[0] < 58)
		return (export_key_syntax_error(s));
	while (s[++i])
	{
		if (s[i] == '_')
			continue ;
		if (!ft_isalnum(s[i]))
			return (export_key_syntax_error(s));
	}
	g_status = 0;
	return (1);
}
/*
void	sort_env(char **env)
{
	char	*tmp;
	int		i;
	int		j;

	i = 0;
	while (env[i])
	{
		j = i + 1;
		while (env[j])
		{
			if (ft_strncmp(env[i], env[j], -1) > 0)
			{
				tmp = env[i];
				env[i] = env[j];
				env[j] = tmp;
			}
			j++;
		}
		i++;
	}
}
*/
void	sort_env(t_env **env_list)
{
	t_env	*next;
	t_env	*cur;
	t_env	swap_tmp;

	cur = *env_list;
	while (cur)
	{
		next = cur->next;
		while (next)
		{
			if (0 < ft_strncmp(cur->key, next->key, -1))
			{
				swap_tmp.key = cur->key;
				swap_tmp.value = cur->value;
				cur->key = next->key;
				cur->value = next->value;
				next->key = swap_tmp.key;
				next->value = swap_tmp.value;
			}
			next = next->next;
		}
		cur = cur->next;
	}
}

int	print_export(t_env *env_list)
{
	sort_env(&env_list);
	while (env_list) //&& env_list->value)
	{
		printf("declare -x %s=\"%s\"\n", env_list->key, env_list->value);
		env_list = env_list->next;
	}
	return (0);
}

int	ft_export(char **buf, t_env *env_list, char **splits, t_env *tmp)
{
	if (!buf[0])
	{
		return (print_export(env_list));
	}
	while (*buf)
	{
		splits = ft_split(*(buf++), '=');
		if (!splits[1] || !export_key_syntax_check(splits[0]))
			continue ;
		tmp = env_dup_check(env_list, splits[0]);
		if (tmp)
		{
			free(tmp->value);
			tmp->value = ft_strdup(splits[1]);
			ft_free_split(splits);
			continue ;
		}
		env_lstadd_back(&env_list, splits[0], splits[1]);
		free(splits);
	}
	return (0);
}
