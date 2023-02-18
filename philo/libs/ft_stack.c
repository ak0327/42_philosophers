/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_stack.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/20 09:19:29 by takira            #+#    #+#             */
/*   Updated: 2023/02/18 09:43:03 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	add_left(t_stack *elem, t_stack **stk)
{
	if (!stk)
		return ;
	elem->prev = NULL;
	elem->next = *stk;
	if (*stk)
		(*stk)->prev = elem;
	*stk = elem;
}

void	add_right(t_stack *elem, t_stack **stk)
{
	t_stack	*last_elem;

	if (!stk)
		return ;
	if (!*stk)
	{
		*stk = elem;
		elem->next = NULL;
		elem->prev = NULL;
		return ;
	}
	last_elem = get_last_elem(*stk);
	last_elem->next = elem;
	elem->prev = last_elem;
	elem->next = NULL;
}

t_stack	*pop_left(t_stack **stk)
{
	t_stack	*first_elem;

	if (!stk || !*stk)
		return (NULL);
	first_elem = *stk;
	*stk = first_elem->next;
	if (first_elem->next)
		first_elem->next->prev = NULL;
	first_elem->prev = NULL;
	first_elem->next = NULL;
	return (first_elem);
}

t_stack	*pop_right(t_stack **stk)
{
	t_stack	*last_elem;

	if (!stk || !*stk)
		return (NULL);
	last_elem = get_last_elem(*stk);
	if (last_elem->prev)
		last_elem->prev->next = NULL;
	else
		*stk = NULL;
	last_elem->prev = NULL;
	last_elem->next = NULL;
	return (last_elem);
}

size_t	get_stack_size(t_stack *stk)
{
	size_t	size;

	size = 0;
	while (stk)
	{
		stk = stk->next;
		size++;
	}
	return (size);
}