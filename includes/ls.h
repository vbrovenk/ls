/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ls.h                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbrovenk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/11 14:02:00 by vbrovenk          #+#    #+#             */
/*   Updated: 2019/05/11 14:02:02 by vbrovenk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LS
# define LS

# include <stdio.h> // for perror

# include "libft.h"
# include <dirent.h>
# include <errno.h>


/*
** end_flags - if flags end before files
*/
typedef struct s_ls
{
	u_int8_t	flag_R;

	u_int8_t	end_flags;

}				t_ls;

# endif
