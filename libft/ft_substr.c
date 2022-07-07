/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wchae <wchae@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/12 17:25:13 by wchae             #+#    #+#             */
/*   Updated: 2022/06/12 17:26:49 by wchae            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(const char *str, size_t start, size_t len)
{
	size_t	i;
	char	*ret;

	ret = malloc(len + 1 * sizeof(char));
	i = 0;
	while (i < len)
	{
		ret[i] = str[i + start];
		i++;
	}
	ret[i] = '\0';
	return (ret);
}