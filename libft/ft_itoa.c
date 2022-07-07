/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wchae <wchae@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/24 18:20:56 by wchae             #+#    #+#             */
/*   Updated: 2022/06/24 18:23:27 by wchae            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	digit_count(int n)
{
	int count;

	count = 1;
	while (n /= 10)
		count++;
	return (count);
}

char		*ft_itoa(int n)
{
	long long	num;
	size_t		size;
	char		*res;

	num = n;
	size = digit_count(n);
	if (num < 0)
		size++;
	if (!(res = (char *)malloc(size + 1)))
		return (NULL);
	res[size] = 0;
	if (num < 0)
	{
		num = -num;
		res[0] = '-';
	}
	res[--size] = (num % 10) + '0';
	while (num /= 10)
		res[--size] = (num % 10) + '0';
	return (res);
}