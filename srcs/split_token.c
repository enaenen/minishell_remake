/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_token.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wchae <wchae@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/09 17:15:54 by wchae             #+#    #+#             */
/*   Updated: 2022/07/09 19:17:43 by wchae            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	find_valid_quot_point(char *data, int start)
{
	int find;

	find = start + 1;
	while (data[find] && data[start] != data[find])
		find++;
	if (data[find])
		return (find);
	return (start);
}
/**
 *
 * 쪼개는 작업
 *  "echo abc > a.txt"
 * 	"echo abc >" AND "a.txt"
 **/

int	split_redirection_token(char *input, int i, t_token **token)
{
	char	*tmp;
	int		save;

	save = i;
	if (i != 0)
	{
		tmp = ft_strntrim(input, " ", i);
		if (!tmp)
			return (error_msg("malloc"));
		env_lstadd_back(token, tmp, NULL);
		input = &input[i];
		i = 0;
	}
	while (input[i] == '<' || input[i] == '>')
		i++;
	tmp = ft_strntrim(input, " ", i);
	if (!tmp)
		return (error_msg("malloc"));
	env_lstadd_back(token, tmp, NULL);
	return (i + save);
}

int	split_space_token(char *input, int i, t_token **token)
{
	char	*tmp;

	if (i != 0)
	{
		tmp = ft_strntrim(input, " ", i);
		if (!tmp)
			return (error_msg("malloc"));
		env_lstadd_back(token, tmp, NULL);
	}
	while (input[i] == ' ')
		i++;
	return (i);
}
/**
 *
 * 쪼개는 작업
 *  "echo abc | ls -l"
 * 	"echo abc" AND "|"" AND "ls -l"
 **/

int		split_pipe_token(char *input, int i, t_token **token)
{
	char	*tmp;

	if (i != 0)
	{
		tmp = ft_strntrim(input, " ", i);
		if (!tmp)
			return (error_msg("malloc"));
		env_lstadd_back(token, tmp, NULL);
	}
	tmp = ft_strdup("|");
	if (!tmp)
		return (error_msg("malloc"));
	env_lstadd_back(token, tmp, NULL);
	return (i + 1);
}

int	split_rest_token(char *input, t_token **token)
{
	char	*tmp;

	if (input[0])
	{
		tmp = ft_strntrim(input, " ", ft_strlen(input));
		if (!tmp)
			return (error_msg("malloc"));
		env_lstadd_back(token, tmp, NULL);
	}
	return (TRUE);
}

int	split_token(char *input, t_token **token)
{
	int	i;

	i = -1;
	while (input[++i])
	{
		if (input[i] == '\"' || input[i] == '\'')
		{
			i = find_valid_quot_point(input, i);
			continue ;
		}
		else if (input[i] == '<' || input[i] == '>')
			i = split_redirection_token(input, i, token);
		else if (input[i] == ' ')
			i = split_space_token(input, i, token);
		else if (input[i] == '|')
			i = split_pipe_token(input, i, token);
		else
			continue ;
		if (i == ERROR)
			return (ERROR);
		input = &input[i];
		i = -1;
	}
	return (split_rest_token(input, token));
}
