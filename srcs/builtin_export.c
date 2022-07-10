/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wchae <wchae@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/04 18:43:23 by wchae             #+#    #+#             */
/*   Updated: 2022/07/10 23:23:49 by wchae            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	export_key_syntax_error(char *s)
{
	write(2, "minishell: export: `", 21);
	write(2, s, ft_strlen(s));
	write(2, "': not a valid identifier\n", 27);
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
	return (1);
}

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

char	**env_key_strs(t_env *env_list)
{
	t_env	*tmp;
	char	**strs;
	int		i;

	tmp = env_list;
	i = 0;
	while (tmp)
	{
		i++;
		tmp = tmp->next;
	}
	strs = malloc(sizeof(char *) * (i + 1));
	if (strs == NULL)
		exit(EXIT_FAILURE);
	strs[i] = NULL;
	i = 0;
	while (env_list)
	{
		strs[i++] = ft_strdup(env_list->key);
		env_list = env_list->next;
	}
	return (strs);
}

void	print_export_env (t_env *env_list, char **tmp)
{
	if (find_env_node(env_list, *tmp)->value)
		printf("declare -x %s=\"%s\"\n", *tmp,
			find_env_node(env_list, *tmp)->value);
	else
		printf("declare -x %s\n", *tmp);
}

int	print_export(t_env *env_list)
{
	char	**env_keys;
	char	**tmp;

	env_keys = env_key_strs(env_list);
	sort_env(env_keys);
	tmp = env_keys;
	while (*tmp)
	{
		if (ft_strncmp(*tmp, "_", -1) != 0)
			print_export_env(env_list, tmp);
		tmp++;
	}
	ft_free_split(env_keys);
	return (0);
}

int	ft_export(char **buf, t_env *env_list)
{
	t_env	*tmp;
	char	*key;
	char	*val;
	char	*eq;
	int		r_flag;

	if (!buf[0])
		return (print_export(env_list));
	r_flag = 0;
	val = NULL;
	while (*buf)
	{
		eq = ft_strchr(*buf, '=');
		if (eq)
		{
			key = ft_substr(*buf, 0, eq - *buf);
			val = ft_substr(*buf, eq - *buf + 1, ft_strlen(eq + 1));
		}
		else
			key = ft_strdup(*buf);
		if (export_key_syntax_check(key) == 0)
		{
			free(key);
			free(val);
			r_flag |= 1;
			buf++;
			val = NULL;
			continue ;
		}
		tmp = find_env_node(env_list, key);
		if (tmp)
		{
			free(key);
			free(tmp->value);
			tmp->value = val;
		}
		else
			env_lstadd_back(&env_list, key, val);
		val = NULL;
		buf++;
	}
	return (r_flag);
}
