/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_lst.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wchae <wchae@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/27 23:51:29 by wchae             #+#    #+#             */
/*   Updated: 2022/07/10 22:50:44 by wchae            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**lst_to_strs(t_list *lst)
{
	t_list	*tmp;
	char	**strs;
	int		i;

	i = 0;
	tmp = lst;
	while (tmp)
	{
		i++;
		tmp = tmp->next;
	}
	strs = malloc(sizeof(char *) * (i + 1));
	strs[i] = NULL;
	i = 0;
	while (lst)
	{
		strs[i++] = ft_strdup(lst->data);
		lst = lst->next;
	}
	return (strs);
}

void	env_lstadd_back(t_env **lst, char *key, char *value)
{
	t_env	*new;
	t_env	*phead;

	new = (t_env *)malloc(sizeof(t_env));
	new->key = key;
	new->value = value;
	new->next = NULL;
	if (*lst == NULL)
	{
		*lst = new;
		return ;
	}
	phead = *lst;
	while (phead->next)
		phead = phead->next;
	phead->next = new;
}

void	env_lstadd_back_node(t_env **lst, t_env	*node)
{
	t_env	*phead;

	if (*lst == NULL)
	{
		*lst = node;
		return ;
	}
	phead = *lst;
	while (phead->next)
		phead = phead->next;
	phead->next = node;
}

char	**tokens_to_strs(t_token *tokens)
{
	t_token	*tmp;
	char	**strs;
	int		i;

	i = 0;
	tmp = tokens;
	while (tmp)
	{
		tmp = tmp->next;
		i++;
	}
	strs = malloc(sizeof(char *) * (i + 1));
	if (strs == NULL)
		exit(EXIT_FAILURE);
	strs[i] = NULL;
	i = 0;
	while (tokens)
	{
		strs[i++] = ft_strdup(tokens->key);
		tokens = tokens->next;
	}
	return (strs);
}

void	del_env_lst(t_env *lst)
{
	t_env	*tmp;

	tmp = lst;
	while (tmp)
	{
		tmp = lst->next;
		free(lst->key);
		free(lst->value);
		free(lst);
		lst = tmp;
	}
}
