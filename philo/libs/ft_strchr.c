/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/17 10:37:26 by takira            #+#    #+#             */
/*   Updated: 2023/02/15 20:01:15 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

char	*ft_strchr(const char *s, int c)
{
	char	chr;
	size_t	i;
	size_t	len;

	chr = (char)c;
	len = ft_strlen_ns(s);
	i = 0;
	while (i <= len)
	{
		if (s[i] == chr)
			return ((char *) &s[i]);
		i++;
	}
	return (NULL);
}
