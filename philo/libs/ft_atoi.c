/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/17 10:31:53 by takira            #+#    #+#             */
/*   Updated: 2023/02/15 20:01:25 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	ft_isspace(char c)
{
	return (ft_strchr("\t\n\v\f\r ", c) != NULL);
}

static bool	is_under_long(long long before_x10_val, long long add_val, int sign)
{
	long long	ov_div;
	long long	ov_mod;

	if (sign == 1)
	{
		ov_div = LONG_MAX / 10;
		ov_mod = LONG_MAX % 10;
	}
	else
	{
		ov_div = ((unsigned long)-LONG_MIN) / 10;
		ov_mod = ((unsigned long)-LONG_MIN) % 10;
	}
	if (before_x10_val > ov_div)
		return (false);
	if (before_x10_val == ov_div && add_val >= ov_mod)
		return (false);
	return (true);
}

static size_t	get_idx_passed_sp_and_sign(const char *str, int *sign)
{
	size_t	i;

	i = 0;
	while (ft_isspace(str[i]))
		i++;
	*sign = 1;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			*sign *= -1;
		i++;
	}
	return (i);
}

static long	ft_strtol(const char *str, size_t *idx, bool *is_success)
{
	long long	ret_num;
	int			sign;

	ret_num = 0;
	sign = 0;
	*idx = get_idx_passed_sp_and_sign(str, &sign);
	if (!ft_isdigit(str[*idx]))
		*is_success = false;
	while (ft_isdigit(str[*idx]))
	{
		if (is_under_long(ret_num, (str[*idx] - '0'), sign) == true)
			ret_num = ret_num * 10 + str[*idx] - '0';
		else if (sign > 0)
			return (LONG_MAX);
		else
			return (LONG_MIN);
		*idx += 1;
	}
	if (str[*idx])
		*is_success = false;
	return ((long)(sign * ret_num));
}

int	ft_atoi(const char *str, bool *is_success)
{
	long long	atol_num;
	size_t		idx;

	*is_success = true;
	atol_num = ft_strtol(str, &idx, is_success);
	if (*is_success && (atol_num < INT_MIN || INT_MAX < atol_num))
		*is_success = false;
	return ((int)(atol_num));
}
