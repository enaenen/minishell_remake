/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export_1.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seseo <seseo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/04 18:43:23 by wchae             #+#    #+#             */
/*   Updated: 2022/07/11 23:06:20 by seseo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	print_export(t_env *env_list);
static void	get_env_key_val(char **buf, char **key, char **val);
static int	export_key_syntax_check(char *s);
static void	set_env_node_export(t_env **env_list, char *key, char *val);

int	ft_export(char **buf, t_env *env_list)
{
	char	*key;
	char	*val;
	int		r_flag;

	if (!buf[0])
		return (print_export(env_list));
	r_flag = 0;
	while (*buf)
	{
		val = NULL;
		get_env_key_val(buf, &key, &val);
		if (export_key_syntax_check(key) != 0)
			set_env_node_export(&env_list, key, val);
		else
		{
			free(key);
			free(val);
			r_flag |= 1;
		}
		buf++;
	}
	return (r_flag);
}

static void	get_env_key_val(char **buf, char **key, char **val)
{
	char	*eq;

	if (**buf == '=')
	{
		*key = ft_strdup(*buf);
		return ;
	}
	eq = ft_strchr(*buf, '=');
	if (eq)
	{
		*key = ft_substr(*buf, 0, eq - *buf);
		*val = ft_substr(*buf, eq - *buf + 1, ft_strlen(eq + 1));
	}
	else
		*key = ft_strdup(*buf);
}

static int	export_key_syntax_check(char *s)
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

static void	set_env_node_export(t_env **env_list, char *key, char *val)
{
	t_env	*tmp;

	tmp = find_env_node(*env_list, key);
	if (tmp)
	{
		free(key);
		if (val)
		{
			free(tmp->value);
			tmp->value = val;
		}
	}
	else
		env_lstadd_back(env_list, key, val);
}

static int	print_export(t_env *env_list)
{
	char	**env_keys;
	char	**tmp;

	env_keys = env_key_strs(env_list);
	sort_env(env_keys);
	tmp = env_keys;
	while (*tmp)
	{
		if (ft_strncmp(*tmp, "_", -1) != 0)
		{
			if (find_env_node(env_list, *tmp)->value)
				printf("declare -x %s=\"%s\"\n", *tmp,
					find_env_node(env_list, *tmp)->value);
			else
				printf("declare -x %s\n", *tmp);
		}
		tmp++;
	}
	ft_free_split(env_keys);
	return (0);
}
