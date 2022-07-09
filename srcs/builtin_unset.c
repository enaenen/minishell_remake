/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seseo <seseo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/09 16:49:19 by wchae             #+#    #+#             */
/*   Updated: 2022/07/10 02:51:11 by seseo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	unset_key_syntax_check(char *s)
{
	int	i;

	i = -1;
	if (ft_isdigit(s[0]))
	{
		write(2, "bash: unset: `", 14);
		write(2, s, ft_strlen(s));
		write(2, "': not a valid identifier\n", 27);
		return (0);
	}
	while (s[++i])
	{
		if (s[i] == '_')
			continue ;
		if (!ft_isalnum(s[i]))
		{
			write(2, "bash: unset: `", 14);
			write(2, s, ft_strlen(s));
			write(2, "': not a valid identifier\n", 27);
			return (0);
		}
	}
	return (1);
}

int	ft_unset(char **buf, t_env **env_list)
{
	int		i;
	t_env	**p_env;
	t_env	*del_env;

	i = -1;
	while (buf[++i])
	{
		if (!unset_key_syntax_check(buf[i]))
			continue ;
		p_env = env_list;
		while (*p_env)
		{
			if (!ft_strcmp(buf[i], (*p_env)->key))
			{
				del_env = *p_env;
				*p_env = ((*p_env)->next);
				free(del_env->key);
				free(del_env->value);
				free(del_env);
			}
			if (*p_env)
				p_env = &(*p_env)->next;
		}
	}
	return (0);
}
