/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wchae <wchae@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/28 00:11:28 by wchae             #+#    #+#             */
/*   Updated: 2022/07/10 22:50:54 by wchae            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_option(char *str)
{
	if (str == NULL)
		return (FALSE);
	if (*str == '-')
	{
		str++;
		while (*str == 'n')
			str++;
	}
	if (*str == 0)
		return (TRUE);
	return (FALSE);
}

int	ft_echo(char **buf)
{
	int	i;
	int	option_flag;

	i = 0;
	option_flag = is_option(*buf);
	while (is_option(*buf))
		buf++;
	while (*buf)
	{
		printf("%s", *buf);
		if (*(buf + 1))
			printf(" ");
		buf++;
	}
	if (!option_flag)
		printf("\n");
	return (0);
}
