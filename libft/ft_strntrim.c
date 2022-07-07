/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strntrim.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wchae <wchae@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/12 17:16:22 by wchae             #+#    #+#             */
/*   Updated: 2022/06/12 18:12:59 by wchae            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strntrim(char *str, char *set, int n)
{
	int		i;
	char	*tmp;
	char	*tmp2;

	tmp = (char *)malloc((n + 1) * sizeof(char));
	if (!tmp)
		return (NULL);
	i = -1;
	while (str[++i] && i < n)
		tmp[i] = str[i];
	tmp[i] = 0;
	tmp2 = ft_strtrim(tmp, set);
	ft_free(tmp);
	return (tmp2);
}