/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wchae <wchae@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/28 18:43:07 by wchae             #+#    #+#             */
/*   Updated: 2022/07/11 12:56:17 by wchae            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	exit_extra_cases(int c, char *s);
static int	valid_exit_arg(char *arg);
static int	exit_num_arg_cal(char *arg);

int	ft_exit(char **buf)
{
	if (!buf[0])
	{
		printf("exit\n");
		exit(0);
	}
	if (!valid_exit_arg(buf[0]))
		return (exit_extra_cases(1, buf[0]));
	else if (!buf[1])
	{
		printf("exit\n");
		exit(exit_num_arg_cal(buf[0]));
	}
	return (exit_extra_cases(2, NULL));
}

int	ft_exit_pipe(char **buf)
{
	if (!buf[0])
		exit(EXIT_SUCCESS);
	if (!valid_exit_arg(buf[0]))
		return (exit_extra_cases(1, buf[0]));
	else if (!buf[1])
		exit(exit_num_arg_cal(buf[0]));
	return (exit_extra_cases(2, NULL));
}

static int	valid_exit_arg(char *arg)
{
	int	i;

	i = 0;
	if (18 < ft_strlen(arg))
		return (0);
	if (arg[0] == '-' || arg[0] == '+')
		i++;
	while (arg[i])
	{
		if (!ft_isdigit(arg[i++]))
			return (0);
	}
	return (1);
}

static int	exit_num_arg_cal(char *arg)
{
	int	ret;

	ret = ft_atoi(arg);
	return (ret);
}

static int	exit_extra_cases(int c, char *s)
{
	if (c == 1)
	{
		printf("exit\n");
		write(2, "minishell: exit ", 17);
		write(2, s, ft_strlen(s));
		write(2, ": numeric argument required\n", 29);
		exit(255);
	}
	else
	{
		printf("exit\n");
		write(2, "minishell: exit: too many arguments\n", 37);
	}
	return (EXIT_FAILURE);
}
