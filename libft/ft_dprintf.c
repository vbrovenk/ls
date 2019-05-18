/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_dprintf.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbrovenk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/18 16:34:38 by vbrovenk          #+#    #+#             */
/*   Updated: 2019/05/18 16:34:41 by vbrovenk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int				ft_dprintf(int fd, const char *format, ...)
{
	va_list		arg;
	int			i;
	int			ret;

	va_start(arg, format);
	ret = 0;
	i = 0;
	g_fd = fd;
	while (format[i])
	{
		if (format[i] == '%')
		{
			i++;
			if (format[i] == '\0')
				break ;
			i = printf_help(format, i, &ret, arg);
		}
		else
		{
			ret++;
			write(g_fd, &format[i], 1);
		}
		i++;
	}
	return (ret);
}
