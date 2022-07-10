/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_pwd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wchae <wchae@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/28 00:14:31 by wchae             #+#    #+#             */
/*   Updated: 2022/07/10 22:50:59 by wchae            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_pwd(void)
{
	char	*buf;

	buf = getcwd(NULL, 0);
	if (buf == NULL)
	{
		write(2, "minishell: pwd: error\n", 23);
		return (EXIT_FAILURE);
	}
	printf("%s\n", buf);
	free (buf);
	return (EXIT_SUCCESS);
}
