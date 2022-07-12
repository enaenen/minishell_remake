/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seseo <seseo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/24 18:20:56 by wchae             #+#    #+#             */
/*   Updated: 2022/07/10 00:18:15 by seseo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	digit_count(int n)
{
	int	count;

	count = 1;
	n /= 10;
	while (n)
	{
		n /= 10;
		count++;
	}
	return (count);
}

char	*ft_itoa(int n)
{
	long long	num;
	size_t		size;
	char		*res;

	num = n;
	size = digit_count(n);
	if (num < 0)
		size++;
	res = (char *)malloc(size + 1);
	if (!res)
		return (NULL);
	res[size] = 0;
	if (num < 0)
	{
		num = -num;
		res[0] = '-';
	}
	res[--size] = (num % 10) + '0';
	num /= 10;
	while (num)
	{
		res[--size] = (num % 10) + '0';
		num /= 10;
	}
	return (res);
}
