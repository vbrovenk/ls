/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   show_files.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbrovenk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/07 12:27:37 by vbrovenk          #+#    #+#             */
/*   Updated: 2019/06/07 12:27:39 by vbrovenk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ls.h"

static	void	count_format_output(t_ls *ls, t_file *files)
{
	t_file	*cur;

	cur = files;
	while (cur != NULL)
	{
		cur->length_nbr_links = ft_countdigits(cur->info->st_nlink);
		if (cur->length_nbr_links > ls->max_length_link)
			ls->max_length_link = cur->length_nbr_links;
		cur->length_nbr_size = ft_countdigits(cur->info->st_size);
		if (cur->length_nbr_size > ls->max_length_size)
			ls->max_length_size = cur->length_nbr_size;
		cur->length_name = ft_strlen(getpwuid(cur->info->st_uid)->pw_name);
		if (cur->length_name > ls->max_length_name)
			ls->max_length_name = cur->length_name;
		cur->length_group = ft_strlen(getgrgid(cur->info->st_gid)->gr_name);
		if (cur->length_group > ls->max_length_group)
			ls->max_length_group = cur->length_group;
		cur->length_major = ft_countdigits((cur->info->st_rdev >> 24) & 0xFF);
		if (cur->length_major > ls->max_length_major)
			ls->max_length_major = cur->length_major;
		ls->total_blocks += cur->info->st_blocks;
		cur = cur->next;
	}
}

void			print_spaces(long long max, long long subtrahend)
{
	int i;

	i = max - subtrahend;
	while (i)
	{
		ft_printf(" ");
		i--;
	}
}

void			print_time(t_file *current)
{
	char *last_modif;
	char **split_date;
	char **split_time;
	long diff;
	char *year;

	last_modif = ctime(&current->info->st_mtimespec.tv_sec);
	split_date = ft_strsplit(last_modif, ' ');
	split_time = ft_strsplit(split_date[3], ':');
	diff = time(NULL) - current->info->st_mtimespec.tv_sec;
	if (diff < -6 * MONTH_SEC || diff > 6 * MONTH_SEC)
	{
		year = ft_strsub(split_date[4], 0, ft_strlen(split_date[4]) - 1);
		ft_printf("%s %2s  %s ", split_date[1], split_date[2], year);
		ft_strdel(&year);
	}
	else
		ft_printf("%s %2s %s:%s ", split_date[1], split_date[2],
									split_time[0], split_time[1]);
	ft_split_clear(split_date);
	ft_split_clear(split_time);
}

void			print_link(t_file *current)
{
	char	*sym_link;
	int		ret;

	sym_link = (char *)ft_memalloc(256);
	if (S_ISLNK(current->info->st_mode))
	{
		ret = readlink(current->full_name, sym_link, 255);
		if (ret == -1)
		{
			ft_dprintf(2, "readlink error \'%s\' - %s\n", current->full_name,
															strerror(errno));
			errno = 0;
		}
		else
			ft_printf(" -> %s", sym_link);
	}
	ft_strdel(&sym_link);
}

void			show_files(char *directory, t_ls *ls, t_file *files,
														int need_dirname)
{
	t_file *current;

	current = files;
	count_format_output(ls, files);
	if (ls->single_arg > 1 && need_dirname == 1)
		ft_printf("%s:\n", directory);
	else if (need_dirname == 1)
		ls->single_arg = 2;
	if (ls->flag_l == 1 && current != NULL && need_dirname == 1)
		ft_printf("total %u\n", ls->total_blocks);
	while (current != NULL)
	{
		if (ls->flag_l == 1)
			show_long(ls, current);
		else
			ft_printf("%s\n", current->name);
		current = current->next;
	}
	ls->max_length_link = 0;
	ls->max_length_name = 0;
	ls->max_length_group = 0;
	ls->max_length_size = 0;
	ls->max_length_major = 0;
	ls->total_blocks = 0;
}
