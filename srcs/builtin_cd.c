/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wchae <wchae@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/28 20:05:39 by wchae             #+#    #+#             */
/*   Updated: 2022/07/09 19:08:46 by wchae            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	cd_error(int error, char *arg)
{
	char	*errmsg;

	errmsg = strerror(error);
	write(2, "bash: cd: ", 10);
	write(2, arg, ft_strlen(arg));
	write(2, ": ", 2);
	write(2, errmsg, ft_strlen(errmsg));
	write(2, "\n", 1);
}

static void	refresh_env_pwd(t_env *env_list, char *pwd, char *old_pwd)
{
	if (pwd && old_pwd)
	{
		if (find_env_node(env_list, "PWD"))
			set_env_node(&env_list, ft_strdup("PWD"), pwd);
		if (find_env_node(env_list, "OLDPWD"))
			set_env_node(&env_list, ft_strdup("OLDPWD"), old_pwd);
	}
}

int	ft_cd(char **buf, t_env *env_list)
{
	int		tmp;
	int		error;
	char	*pwd;
	char	*old_pwd;

	old_pwd = getcwd(NULL, 0);
	if (!buf[0])
	{
		chdir(read_key(env_list, "HOME"));
		g_status = 0;
		return (EXIT_SUCCESS);
	}
	tmp = chdir(buf[0]);
	if (tmp < 0)
	{
		error = errno;
		cd_error(error, buf[0]);
		g_status = error;
		return (EXIT_FAILURE);
	}
	pwd = getcwd(NULL, 0);
	if (pwd && old_pwd)
		refresh_env_pwd(env_list, pwd, old_pwd);
	return (EXIT_SUCCESS);
}
