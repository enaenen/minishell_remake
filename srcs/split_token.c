/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_token.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wchae <wchae@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/09 17:15:54 by wchae             #+#    #+#             */
/*   Updated: 2022/07/10 22:50:47 by wchae            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

int	split_pipe_token(char *input, int i, t_token **token)
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
