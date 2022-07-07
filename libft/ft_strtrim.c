/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wchae <wchae@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/12 17:21:03 by wchae             #+#    #+#             */
/*   Updated: 2022/06/12 17:56:59 by wchae            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	*ft_memchr(const void *s, int c, size_t n)
{
	char	*cp;

	cp = (char *)s;
	while (n--)
	{
		if (*cp == (char) c)
			return ((void *) cp);
		cp++;
	}
	return (NULL);
}

char	*ft_strtrim(char const *str, char const *set)
{
	char		*ret;
	char const	*start;
	char const	*end;
	size_t		str_len;
	size_t		set_len;

	str_len = ft_strlen(str);
	set_len = ft_strlen(set);
	start = str;
	end = str + str_len -1;
	while (start < end && ft_memchr(set, *start, set_len))
		start++;
	while (start <= end && ft_memchr(set, *end, set_len))
		end--;
	ret = ft_substr(str, start - str, end - start + 1);
	return (ret);
}