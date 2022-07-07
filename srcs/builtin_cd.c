/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wchae <wchae@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/28 20:05:39 by wchae             #+#    #+#             */
/*   Updated: 2022/07/04 15:41:28 by wchae            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static char *read_key(t_env *env_list, char *key)
{
	while (env_list)
	{
		if (ft_strcmp(env_list->key, key) == 0)
			return (ft_strdup(env_list->value));
		env_list = env_list->next;
	}
	return (NULL);
}

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



void	ft_cd(char **buf, t_env *env_list)
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
		return ;
	}
	tmp = chdir(buf[0]);
	if (tmp < 0)
	{
		//TODO $PWD , $OLD_PWD 갱신
		error = errno;
		cd_error(error, buf[0]);
		g_status = error;
		return ;
	}
	pwd = getcwd(NULL, 0);
	if(pwd && old_pwd)
	{
		if (find_env_node(env_list, "PWD"))
			set_env_node(&env_list, ft_strdup("PWD"), pwd);
		if (find_env_node(env_list, "OLDPWD"))
			set_env_node(&env_list, ft_strdup("OLDPWD"), old_pwd);
	}
	g_status = 0;
}

/*

static char	*get_env_val(t_env *env_list, char *envkey)
{
	t_env	*dir_node;

	dir_node = find_env_node(env_list, envkey);
	if (dir_node)
		return (dir_node->value);
	return (NULL);
}

static void	print_err_no_env(char *env_name)
{
	t_buffer	*buf;
	char		*err_msg;

	buf = create_buf();
	add_str(buf, "minishell");
	add_str(buf, ": cd: ");
	add_str(buf, env_name);
	add_str(buf, " not set\n");
	err_msg = put_str(buf);
	ft_putstr_fd(err_msg, STDERR_FILENO);
	del_buf(buf);
	free(err_msg);
}

static int	change_dir_from_env(t_proc *proc, char *env_key)
{
	const char	*dir = get_env_val(proc->env_list, env_key);
	char		*tmp;

	if (dir == NULL)
	{
		print_err_no_env(env_key);
		return (1);
	}
	tmp = ft_strdup(proc->cur_path);
	if (chdir(dir) == 0)
	{
		if (ft_strncmp(env_key, "OLDPWD", -1) == 0)
			printf("%s\n", dir);
		set_env_node(&proc->env_list, ft_strdup("OLDPWD"), tmp);
		if (find_env_node(proc->env_list, "PWD"))
			set_env_node(&proc->env_list, ft_strdup("PWD"), getcwd(NULL, 0));
		free(proc->cur_path);
		proc->cur_path = getcwd(NULL, 0);
		return (0);
	}
	free(tmp);
	// print_err_msg("cd", strerror(errno));
	return (1);
}

static int	change_dir_input(t_proc *proc, char *dir)
{
	t_buffer	*buf;
	char		*tmp;
	char		*errmsg;
	int			error;

	tmp = ft_strdup(proc->cur_path);
	if (chdir(dir) == 0)
	{
		set_env_node(&proc->env_list, ft_strdup("OLDPWD"), tmp);
		if (find_env_node(proc->env_list, "PWD"))
			set_env_node(&proc->env_list, ft_strdup("PWD"), getcwd(NULL, 0));
		free(proc->cur_path);
		proc->cur_path = getcwd(NULL, 0);
		return (EXIT_SUCCESS);
	}

	
	free(tmp);
	// cd_error(error, buf[0]);
	// g_status = error;
	buf = create_buf();
	add_str(buf, "cd: ");
	add_str(buf, dir);
	errmsg = put_str(buf);
	error = errno;
	// print_err_msg(errmsg, strerror(errno));
	free(errmsg);
	del_buf(buf);
	return (EXIT_FAILURE);
}

*/
/**
int		ft_cd2(t_proc *proc, char **cmd)
{
	char	*dir;
	if (cmd[0])
	{
		if (ft_strncmp(cmd[1], "~/", 2) == 0)
		{
			dir = ft_strjoin(getenv("HOME"), &cmd[1][1]);
			return (change_dir_input()Ż
		}
			return (change_dir_input(proc, getenv("HOME")));
		else if (ft_strncmp(cmd[1], "--", -1) == 0)
			return (change_dir_from_env(proc, "HOME"));
		else if (ft_strncmp(cmd[1], "-", -1) == 0)
			return (change_dir_from_env(proc, "OLDPWD"));
		return (change_dir_input(proc, cmd[1]));
	}
	return 0;
}
**/