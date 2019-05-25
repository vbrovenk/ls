/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_clear.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbrovenk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/25 15:25:51 by vbrovenk          #+#    #+#             */
/*   Updated: 2019/05/25 15:25:53 by vbrovenk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_split_clear(char **split_str)
{
	int i;

	i = 0;
	while (split_str[i] != NULL)
	{
		free(split_str[i]);
		i++;
	}
	free(split_str);
}
