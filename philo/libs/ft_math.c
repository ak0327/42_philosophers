/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_math.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/20 09:59:22 by takira            #+#    #+#             */
/*   Updated: 2023/02/20 09:59:51 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

size_t	min_size(size_t a, size_t b)
{
	if (a <= b)
		return (a);
	return (b);
}

size_t	max_size(size_t a, size_t b)
{
	if (a >= b)
		return (a);
	return (b);
}

