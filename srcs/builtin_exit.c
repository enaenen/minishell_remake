/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wchae <wchae@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/28 18:43:07 by wchae             #+#    #+#             */
/*   Updated: 2022/06/28 19:56:16 by wchae            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	exit_extra_cases(int c, char *s)
{
	if (c == 1)
	{
		printf("exit\n");
		write(2,"bash: exit ", 12);
		write(2, s, ft_strlen(s));
		write(2, ": numeric argument, required\n", 28);
		exit(255);
	}
	else
	{
		printf("exit\n");
		write(2, "bash: exit: too many arguments\n", 31);
		g_status = 1;
	}
}

int valid_exit_arg(char *arg)
{
	int i;
	
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

int		exit_num_arg_cal(char *arg)
{
	int	ret;

	ret = ft_atoi(arg);
	if (0 <= ret && ret < 255)
		return (ret);
	else
		return (255);
}

void	ft_exit(char **buf)
{
	if (!buf[0])
		exit(0);
	if (!valid_exit_arg(buf[0]))
		exit_extra_cases(1, buf[0]);
	else if (!buf[1])
	{
		printf("exit\n");
		exit(exit_num_arg_cal(buf[0]));
	}
	else
	{
		exit_extra_cases(2, NULL);
		return ;
	}
}