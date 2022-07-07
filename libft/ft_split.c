/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wchae <wchae@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/12 14:05:43 by wchae             #+#    #+#             */
/*   Updated: 2022/06/12 18:14:49 by wchae            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	*ft_spit_free(char **str, int j)
{
	int	i;

	i = 0;
	while (i <= j)
	{
		free(str[i]);
		i++;
	}
	free(str);
	return (NULL);
}

static int	ft_str_wdcnt(char const *s, char c)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (s[i])
	{
		while (s[i] && s[i] == c)
			i++;
		if (s[i] && s[i] != c)
		{
			count++;
			while (s[i] && s[i] != c)
				i++;
		}
	}
	return (count);
}

static char	**ft_alloc_chr(char **ret, char const *s, char c)
{
	int i;
	int j;
	int cnt;

	i = 0;
	j = -1;
	while (s[i])
	{
		cnt = 0;
		while (s[i] && s[i] == c)
			i++;
		if (s[i] && s[i] != c)
		{
			j++;
			while (s[i] && s[i] != c)
			{
				cnt++;
				i++;
			}
			ret[j] = (char *)malloc((cnt + 1) * sizeof(char));
			if (!ret[j])
				return (ft_spit_free(ret, j - 1));
		}
	}
	return (ret);
}

static void	ft_strcpy(char **dest, char const *src, char c)
{
	int	i;
	int	j;
	int	k;

	i = 0;
	j = -1;
	while (src[i])
	{
		k = 0;
		while (src[i] && src[i] == c)
			i++;
		if (src[i] && src[i] != c)
		{
			j++;
			while (src[i] && src[i] != c)
			{
				dest[j][k] = src[i];
				i++;
				k++;
			}
			dest[j][k] = '\0';
		}
	}
}

char	**ft_split(char const *str, char c)
{
	char **ret;
	int	word_cnt;

	if (!str)
		return (NULL);
	word_cnt = ft_str_wdcnt(str, c);
	ret = (char **)malloc((word_cnt + 1) * sizeof(char *));
	if (!ret)
		return (NULL);
	if (ft_alloc_chr(ret, str, c) == NULL)
		return (NULL);
	ret[word_cnt] = NULL;
	ft_strcpy(ret, str, c);
	return (ret);
}