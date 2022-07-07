/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lst_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seseo <seseo@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/27 23:51:29 by wchae             #+#    #+#             */
/*   Updated: 2022/07/07 21:28:54 by seseo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int		ft_lstsize(t_list *lst)
{
	int	i;

	i = 0;
	while (lst)
	{
		lst = lst->next;
		i++;
	}
	return (i);
}

t_list	*ft_lstlast(t_list *lst)
{
	if (!lst)
		return (NULL);
	while (lst->next)
		lst = lst->next;
	return (lst);
}

void	ft_lstadd_back(t_list **lst, t_list *new)
{
	if (!lst || !new)
		return ;
	if (!*lst)
		*lst = new;
	else
		ft_lstlast(*lst)->next = new;
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

t_list	*ft_lstnew(void *data)
{
	t_list	*new;

	new = (t_list *)malloc(sizeof(t_list));
	if (!new)
		return (NULL);
	new->data = data;
	new->next = NULL;
	return (new);
}

void	ft_lstclear(t_list **lst, void (*del)(void *))
{
	t_list	*next;

	if (!lst || !*lst || !del)
		return ;
	while (*lst)
	{
		next = (*lst)->next;
		del((*lst)->data);
		(*lst)->data = NULL;
		free(*lst);
		*lst = NULL;
		*lst = next;
	}
	*lst = NULL;
}
