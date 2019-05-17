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
	// struct dirent *entry;
	
	char	*name;
	
	struct s_file *next;

}				t_file;



/*
** args_files - if files pass in args of ls
*/
typedef struct s_ls
{
	u_int8_t	flag_R;
	u_int8_t	flag_a;
	u_int8_t	flag_r;

	u_int8_t	args_files;

	t_file		*list_files;
	t_file		*last_file;

	t_file		*dirs;
	t_file		*non_dirs;

}				t_ls;


/*
** sorting.c
*/
void	merge_sort(t_ls *ls, t_file **list_files);

# endif
