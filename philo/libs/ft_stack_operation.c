/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_stack_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/27 18:28:00 by takira            #+#    #+#             */
/*   Updated: 2023/02/22 22:58:40 by takira           ###   ########.fr       */
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

void	swap(t_stack **stack)
{
	t_stack	*first_elem;
	t_stack	*second_elem;

	if (!stack || get_stack_size(*stack) < 2)
		return ;
	first_elem = *stack;
	second_elem = first_elem->next;
	first_elem->prev = second_elem;
	first_elem->next = second_elem->next;
	if (second_elem->next)
		second_elem->next->prev = first_elem;
	second_elem->prev = NULL;
	second_elem->next = first_elem;
	*stack = second_elem;
}
