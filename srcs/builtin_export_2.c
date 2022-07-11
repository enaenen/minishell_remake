/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export_2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seseo <seseo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/11 10:13:48 by seseo             #+#    #+#             */
/*   Updated: 2022/07/11 10:14:04 by seseo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

int	export_key_syntax_error(char *s)
{
	write(2, "minishell: export: `", 21);
	write(2, s, ft_strlen(s));
	write(2, "': not a valid identifier\n", 27);
	return (0);
}
