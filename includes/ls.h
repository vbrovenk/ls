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
** struct for save 
*/
typedef struct s_file
{
	struct dirent *entry;
	
	struct s_file *next;

}				t_file;

typedef	struct s_args
{
	char *name;
	struct s_args *next;

}				t_args;


/*
** args_files - if files pass in args of ls
*/
typedef struct s_ls
{
	u_int8_t	flag_R;

	u_int8_t	args_files;

	t_file		*list_files;
	t_file		*last_file;

	t_args		*dirs;
	t_args		*non_dirs;

}				t_ls;




# endif
