/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   choose_sort.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbrovenk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/07 13:27:10 by vbrovenk          #+#    #+#             */
/*   Updated: 2019/06/07 13:27:11 by vbrovenk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ls.h"

void	show_user_group(t_ls *ls, t_file *current)
{
	errno = 0;
	ls->usr = getpwuid(current->info->st_uid);
	if (ls->usr == NULL)
	{
		ft_dprintf(2, "getpwuid(): %s\n", strerror(errno));
		ft_printf("%u  ", current->info->st_uid);
		errno = 0;
	}
	else
	{
		ft_printf("%s  ", ls->usr->pw_name);
		print_spaces(ls->max_length_name, current->length_name);
	}
	ls->grp = getgrgid(current->info->st_gid);
	if (ls->grp == NULL)
	{
		ft_dprintf(2, "getgrgid(): %s\n", strerror(errno));
		ft_printf("%u  ", current->info->st_gid);
		errno = 0;
	}
	else
	{
		ft_printf("%s  ", ls->grp->gr_name);
		print_spaces(ls->max_length_group, current->length_group);
	}
}

void	choose_sort(t_ls *ls, t_file **list_files)
{
	merge_sort(ls, list_files, merge_sorted_list);
	if (ls->flag_t == 1)
		merge_sort(ls, list_files, merge_time_list);
	if (ls->flag_r == 1)
		merge_sort(ls, list_files, merge_reverse_list);
}
