/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wchae <wchae@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/03 20:56:47 by wchae             #+#    #+#             */
/*   Updated: 2022/07/03 21:10:13 by wchae            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_env(t_env *env_list)
{
	set_env_node(&env_list, ft_strdup("_"), ft_strdup("/usr/bin/env"));
	while (env_list)
	{
		if(env_list->value != NULL)
			printf("%s=%s\n", (char *)env_list->key, env_list->value);
		env_list = env_list->next;
	}
	return (0);
}