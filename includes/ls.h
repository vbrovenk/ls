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

#ifndef LS_H
# define LS_H

# include "libft.h"
# include <dirent.h>
# include <errno.h>
# include <sys/stat.h>
# include <pwd.h>
# include <grp.h>
# include <time.h>

# define MONTH_SEC 2678400

typedef struct	s_file
{
	char			*name;
	char			*full_name;
	struct stat		*info;
	u_int32_t		length_nbr_links;
	u_int32_t		length_name;
	u_int32_t		length_group;
	u_int32_t		length_nbr_size;
	u_int32_t		length_major;
	struct s_file	*next;

}				t_file;

/*
** args_files - if files pass in args of ls
** single_arg - if only one arg-file, don't need show name of this dir
*/
typedef struct	s_ls
{
	u_int8_t		recursion;
	u_int8_t		flag_a;
	u_int8_t		flag_r;
	u_int8_t		flag_l;
	u_int8_t		flag_t;
	u_int8_t		args_files;
	u_int8_t		single_arg;
	u_int64_t		total_blocks;
	u_int32_t		max_length_link;
	u_int32_t		max_length_name;
	u_int32_t		max_length_group;
	u_int32_t		max_length_size;
	u_int32_t		max_length_major;
	t_file			*list_files;
	t_file			*last_file;
	t_file			*dirs;
	t_file			*non_dirs;
	struct passwd	*usr;
	struct group	*grp;
}				t_ls;

/*
** sorting.c
*/
void			merge_sort(t_ls *ls, t_file **list_files,
								t_file *(*sorting) (t_file *, t_file *));
t_file			*merge_sorted_list(t_file *left, t_file *right);
t_file			*merge_reverse_list(t_file *left, t_file *right);
t_file			*merge_time_list(t_file *left, t_file *right);

/*
** show_files.c
*/
void			show_files(char *directory, t_ls *ls, t_file *files,
														int need_dirname);
void			print_spaces(long long max, long long subtrahend);
void			print_time(t_file *current);
void			print_link(t_file *current);

/*
** show_long.c
*/
void			show_long(t_ls *ls, t_file *current);

/*
** show_directory.c
*/
void			open_directory(t_ls *ls, char *full_name, char *dir_name);

/*
** work_with_list.c
*/
t_file			*free_list_files(t_file *list_files);
void			add_file_to_list(t_ls *ls, struct dirent *entry,
														char *full_name);
void			add_to_list(t_file **head, char *name, char *full_name);
void			ft_error(char *message);

/*
** choose_sort.c
*/
void			choose_sort(t_ls *ls, t_file **list_files);
void			show_user_group(t_ls *ls, t_file *current);

#endif
