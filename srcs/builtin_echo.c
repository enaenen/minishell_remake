/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wchae <wchae@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/28 00:11:28 by wchae             #+#    #+#             */
/*   Updated: 2022/06/28 15:23:06 by wchae            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_echo(char **buf)
{
	int	i;

	i = 0;
	while (buf[i] != NULL && !ft_strcmp(buf[i], "-n"))
		i++;
	while (buf[i])
	{
		printf("%s", buf[i]);
		if (buf[i++ + 1] != 0)
			printf(" ");
	}
	if ((i == 0) || (0 < i && ft_strcmp(buf[0], "-n") != 0))
		printf("\n");
	g_status = 0;
	return (0);
}