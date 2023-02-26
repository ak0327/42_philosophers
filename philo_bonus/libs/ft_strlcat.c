/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/26 10:18:50 by takira            #+#    #+#             */
/*   Updated: 2023/02/26 10:19:24 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

static void	*ft_memcpy(void *dst, const void *src, size_t n)
{
	unsigned char		*dst_cpy;
	const unsigned char	*src_cpy;
	size_t				i;
	size_t				j;

	if (dst == src)
		return (dst);
	dst_cpy = (unsigned char *)dst;
	src_cpy = (const unsigned char *)src;
	i = 0;
	j = 0;
	while (i < n)
		dst_cpy[i++] = src_cpy[j++];
	return (dst);
}

size_t	ft_strlcat_ns(char *dst, const char *src, size_t dstsize)
{
	size_t	dst_len;
	size_t	src_len;
	size_t	cat_len;

	src_len = ft_strlen_ns(src);
	if (dstsize == 0)
		return (src_len);
	dst_len = ft_strlen_ns(dst);
	if (dstsize <= dst_len)
		return (src_len + dstsize);
	cat_len = min_size(dstsize - dst_len - 1, src_len);
	ft_memcpy(&dst[dst_len], &src[0], cat_len);
	dst[dst_len + cat_len] = '\0';
	return (dst_len + src_len);
}
