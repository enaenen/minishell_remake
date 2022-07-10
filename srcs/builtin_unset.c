/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seseo <seseo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/09 16:49:19 by wchae             #+#    #+#             */
/*   Updated: 2022/07/10 18:35:45 by seseo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	unset_key_syntax_check(char *s);

int	ft_unset(char **buf, t_env **env_list)
{
	t_env	**p_env;
	t_env	*del_env;
	int		r_flag;

	r_flag = 0;
	while (*buf)
	{
		if (!unset_key_syntax_check(*buf))
		{
			r_flag |= 1;
			buf++;
			continue ;
		}
		p_env = env_list;
		while (*p_env)
		{
			if (!ft_strcmp(*buf, (*p_env)->key))
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
		buf++;
	}
	return (r_flag);
}

static int	unset_key_syntax_check(char *s)
{
	int	i;

	i = -1;
	if (ft_isdigit(s[0]))
	{
		write(2, "minishell: unset: `", 20);
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
			write(2, "minishell: unset: `", 20);
			write(2, s, ft_strlen(s));
			write(2, "': not a valid identifier\n", 27);
			return (0);
		}
	}
	return (1);
}
