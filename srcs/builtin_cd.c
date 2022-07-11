/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wchae <wchae@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/28 20:05:39 by wchae             #+#    #+#             */
/*   Updated: 2022/07/11 16:06:38 by wchae            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	print_err_no_env(char *env_name);
static int	cd_error(int error, char *arg);
static void	refresh_env_pwd(t_env *env_list, char *pwd, char *old_pwd);

int	ft_cd(char **buf, t_env *env_list)
{
	int		error;
	char	*dir;
	char	*pwd;
	char	*old_pwd;

	if (!buf[0])
	{
		if (find_env_node(env_list, "HOME"))
			dir = find_env_node(env_list, "HOME")->value;
		else
			return (print_err_no_env("HOME"));
	}
	else
		dir = buf[0];
	old_pwd = getcwd(NULL, 0);
	if (chdir(dir))
	{
		error = errno;
		free(old_pwd);
		return (cd_error(error, dir));
	}
	pwd = getcwd(NULL, 0);
	if (pwd && old_pwd)
		refresh_env_pwd(env_list, pwd, old_pwd);
	return (EXIT_SUCCESS);
}

static int	print_err_no_env(char *env_name)
{
	t_buffer	*buf;
	char		*err_msg;

	buf = create_buf();
	add_str(buf, "minishell: cd: ");
	add_str(buf, env_name);
	add_str(buf, " not set\n");
	err_msg = put_str(buf);
	ft_putstr_fd(err_msg, STDERR_FILENO);
	del_buf(buf);
	free(err_msg);
	return (EXIT_FAILURE);
}

static int	cd_error(int error, char *arg)
{
	char	*errmsg;

	errmsg = strerror(error);
	write(2, "minishell: cd: ", 16);
	write(2, arg, ft_strlen(arg));
	write(2, ": ", 2);
	write(2, errmsg, ft_strlen(errmsg));
	write(2, "\n", 1);
	return (EXIT_FAILURE);
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
