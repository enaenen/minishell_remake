/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strndup.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wchae <wchae@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/22 02:03:41 by wchae             #+#    #+#             */
/*   Updated: 2022/06/22 02:05:21 by wchae            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strndup(char *s, int n)
{
	char	*ret;
	int		i;

	ret = (char *)malloc((n + 1) * sizeof(char));
	if (!ret)
		return (NULL);
	i = -1;
	while (s[++i] && i < n)
		ret[i] = s[i];
	ret[i] = '\0';
	return (ret);
}
