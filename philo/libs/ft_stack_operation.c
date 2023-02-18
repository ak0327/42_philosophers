/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_stack_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/27 18:28:00 by takira            #+#    #+#             */
/*   Updated: 2023/02/18 09:37:20 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

t_stack	*create_stack_elem(size_t idx)
{
	t_stack	*new_elem;

	new_elem = (t_stack *)malloc(sizeof(t_stack));
	if (!new_elem)
		return (NULL);
	new_elem->idx = idx;
	new_elem->prev = NULL;
	new_elem->next = NULL;
	return (new_elem);
}

t_stack	*get_last_elem(t_stack *elem)
{
	if (!elem)
		return (NULL);
	while (elem->next)
		elem = elem->next;
	return (elem);
}

void	ft_stack_clear(t_stack **stk)
{
	t_stack	*next;

	if (!stk)
		return ;
	while (*stk)
	{
		next = (*stk)->next;
		free(*stk);
		*stk = next;
	}
	*stk = NULL;
}
