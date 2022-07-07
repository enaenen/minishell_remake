/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wchae <wchae@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/28 17:23:24 by wchae             #+#    #+#             */
/*   Updated: 2022/06/28 17:24:24 by wchae            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"


void	*ft_memcpy(void *dst, const void *src, size_t n)
{
	size_t index;

	index = -1;
	if (n && dst != src)
	{
		while (++index < n)
			((unsigned char *)dst)[index] = ((unsigned char *)src)[index];
	}
	return (dst);
}
