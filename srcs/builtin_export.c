/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seseo <seseo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/04 18:43:23 by wchae             #+#    #+#             */
/*   Updated: 2022/07/09 15:50:37 by seseo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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

int	print_export(t_env *env_list)
{
	char	**env;

	env = get_env_list(&env_list);
	sort_env(env);
	while (*env)
	{
		printf("declare -x %s=\"%s\"\n", *env, find_env_node(env_list, *env)->value);
		env++;
	}
	ft_free_split(env);
	return (0);
}

int	ft_export(char **buf, t_env *env_list, char **splits, t_env *tmp)
{
	if (!buf[0])
	{
		g_status = 0;
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
