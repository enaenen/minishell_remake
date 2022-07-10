/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_pwd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seseo <seseo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/28 00:14:31 by wchae             #+#    #+#             */
/*   Updated: 2022/07/10 16:51:08 by seseo            ###   ########.fr       */
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
