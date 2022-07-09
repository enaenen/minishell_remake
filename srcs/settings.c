/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   settings.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wchae <wchae@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/27 23:53:43 by wchae             #+#    #+#             */
/*   Updated: 2022/07/09 16:44:05 by wchae            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_env	*env_set(char	**envp)
{
	t_env	*env_list;
	char	*eq;

	env_list = NULL;
	while (*envp)
	{
		eq = ft_strchr(*envp, '=');
		if (eq)
			env_lstadd_back(&env_list,
				ft_substr(*envp, 0, eq - *envp),
				ft_substr(*envp, eq - *envp + 1,
					ft_strlen(eq + 1)));
		envp++;
	}
	return (env_list);
}

void	set_env_node(t_env **env, char *key, char *val)
{
	t_env	*tmp;

	tmp = find_env_node(*env, key);
	if (tmp)
	{
		free(key);
		if (tmp->value)
			free(tmp->value);
		tmp->value = val;
	}
	else
		env_lstadd_back(env, key, val);
}

static void	env_init(t_env **env)
{
	t_env		*shlvl;
	int			lvl;

	set_env_node(env, ft_strdup("PWD"), getcwd(NULL, 0));
	set_env_node(env, ft_strdup("OLDPWD"), NULL);
	shlvl = find_env_node(*env, "SHLVL");
	if (shlvl)
	{
		lvl = ft_atoi(shlvl->value);
		set_env_node(env, ft_strdup("SHLVL"), ft_itoa(++lvl));
	}
	else
		set_env_node(env, ft_strdup("SHLVL"), ft_itoa(1));
}

void	init_set(t_set *set, t_env **env)
{
	ft_memset(set, 0, sizeof(t_set));
	g_status = 0;
	set->org_stdin = dup(STDIN_FILENO);
	set->org_stdout = dup(STDOUT_FILENO);
	tcgetattr(STDIN_FILENO, &set->org_term);
	tcgetattr(STDIN_FILENO, &set->new_term);
	set->new_term.c_lflag &= ~(ECHOCTL);
	set->org_term.c_lflag |= ECHOCTL;
	*env = env_set(environ);
	env_init(env);
}
