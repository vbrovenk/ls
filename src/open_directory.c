/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   open_directory.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbrovenk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/07 13:02:17 by vbrovenk          #+#    #+#             */
/*   Updated: 2019/06/07 13:02:18 by vbrovenk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ls.h"

static	char	*join_path(char *part_path, char *current_dir)
{
	char *temp;
	char *full_path;

	full_path = ft_strjoin(part_path, "/");
	temp = full_path;
	full_path = ft_strjoin(full_path, current_dir);
	ft_strdel(&temp);
	return (full_path);
}

static	int		access_error(char *full_name, char *dir_name)
{
	if (errno == EACCES)
	{
		ft_printf("%s:\n", full_name);
		ft_dprintf(2, "ft_ls: %s : %s\n", dir_name, strerror(errno));
		errno = 0;
		return (1);
	}
	return (0);
}

static	t_file	*sort_and_show(t_ls *ls, DIR *dir, char *dir_name,
																t_file **dirs)
{
	choose_sort(ls, &ls->list_files);
	choose_sort(ls, dirs);
	closedir(dir);
	show_files(dir_name, ls, ls->list_files, 1);
	ls->list_files = free_list_files(ls->list_files);
	return (*dirs);
}

static	void	adding_file(t_ls *ls, struct dirent *entry, t_file **dirs,
														char *path_to_file)
{
	char *full_name;

	if (entry->d_type == DT_DIR && ft_strequ(entry->d_name, ".") == 0
								&& ft_strequ(entry->d_name, "..") == 0)
	{
		if (ls->flag_a == 1 || entry->d_name[0] != '.')
		{
			full_name = join_path(path_to_file, entry->d_name);
			add_to_list(dirs, entry->d_name, full_name);
		}
	}
	if (ls->flag_a == 1 || entry->d_name[0] != '.')
	{
		full_name = join_path(path_to_file, entry->d_name);
		add_file_to_list(ls, entry, full_name);
	}
}

void			open_directory(t_ls *ls, char *full_name, char *dir_name)
{
	DIR				*dir;
	struct dirent	*entry;
	t_file			*nested_dirs;
	t_file			*current_dir;
	char			*full_path;

	dir = opendir(full_name);
	nested_dirs = NULL;
	if (access_error(full_name, dir_name) == 1)
		return ;
	while ((entry = readdir(dir)) != NULL)
		adding_file(ls, entry, &nested_dirs, full_name);
	current_dir = sort_and_show(ls, dir, full_name, &nested_dirs);
	if (ls->recursion == 1)
	{
		while (current_dir != NULL)
		{
			ft_printf("\n");
			full_path = join_path(full_name, current_dir->name);
			open_directory(ls, full_path, current_dir->name);
			current_dir = current_dir->next;
			ft_strdel(&full_path);
		}
	}
	free_list_files(nested_dirs);
}
