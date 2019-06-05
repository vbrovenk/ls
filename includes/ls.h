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
# include <sys/stat.h>
# include <pwd.h>
# include <grp.h>
# include <time.h>


# define MONTH_SEC 2678400

/*
** struct for save 
*/
typedef struct s_file
{
	// struct dirent *entry;
	
	char	*name;
	char	*full_name;
	struct stat *info;
	u_int32_t	length_nbr_links;
	u_int32_t	length_name;
	u_int32_t	length_group;
	u_int32_t	length_nbr_size;

	struct s_file *next;

}				t_file;



/*
** args_files - if files pass in args of ls
** single_arg - if only one arg-file, don't need show name of this dir
** max_length_link - length of max number st_link; using for output 
*/
typedef struct s_ls
{
	u_int8_t	flag_R;
	u_int8_t	flag_a;
	u_int8_t	flag_r;
	u_int8_t	flag_l;
	u_int8_t	flag_t;

	u_int8_t	args_files;
	u_int8_t	single_arg;

	u_int64_t	total_blocks;

	u_int32_t	max_length_link;
	u_int32_t	max_length_name;
	u_int32_t	max_length_group;
	u_int32_t	max_length_size;

	t_file		*list_files;
	t_file		*last_file;

	t_file		*dirs;
	t_file		*non_dirs;

}				t_ls;


/*
** sorting.c
*/
// void	merge_sort(t_ls *ls, t_file **list_files);

void	choose_sort(t_ls *ls, t_file **list_files);

# endif
