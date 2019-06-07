/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   show_long.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbrovenk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/07 12:42:58 by vbrovenk          #+#    #+#             */
/*   Updated: 2019/06/07 12:43:00 by vbrovenk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ls.h"

static	void	print_type_file(t_file *current)
{
	if (S_ISBLK(current->info->st_mode))
		ft_printf("b");
	else if (S_ISCHR(current->info->st_mode))
		ft_printf("c");
	else if (S_ISDIR(current->info->st_mode))
		ft_printf("d");
	else if (S_ISLNK(current->info->st_mode))
		ft_printf("l");
	else if (S_ISSOCK(current->info->st_mode))
		ft_printf("s");
	else if (S_ISFIFO(current->info->st_mode))
		ft_printf("p");
	else
		ft_printf("-");
}

static	void	print_other_mode(t_file *current)
{
	ft_printf((current->info->st_mode & S_IROTH) ? "r" : "-");
	ft_printf((current->info->st_mode & S_IWOTH) ? "w" : "-");
	if ((current->info->st_mode & S_IXOTH) &&
										(current->info->st_mode & S_ISVTX))
		ft_printf("t  ");
	else if ((current->info->st_mode & S_ISVTX) &&
										!(current->info->st_mode & S_IXOTH))
		ft_printf("T  ");
	else if (current->info->st_mode & S_IXOTH)
		ft_printf("x  ");
	else
		ft_printf("-  ");
}

static	void	print_group_mode(t_file *current)
{
	ft_printf((current->info->st_mode & S_IRGRP) ? "r" : "-");
	ft_printf((current->info->st_mode & S_IWGRP) ? "w" : "-");
	if ((current->info->st_mode & S_IXGRP) &&
										(current->info->st_mode & S_ISGID))
		ft_printf("s");
	else if (!(current->info->st_mode & S_IXGRP) &&
										(current->info->st_mode & S_ISGID))
		ft_printf("S");
	else if (current->info->st_mode & S_IXGRP)
		ft_printf("x");
	else
		ft_printf("-");
	print_other_mode(current);
}

static	void	print_user_mode(t_file *current)
{
	ft_printf((current->info->st_mode & S_IRUSR) ? "r" : "-");
	ft_printf((current->info->st_mode & S_IWUSR) ? "w" : "-");
	if ((current->info->st_mode & S_IXUSR) &&
										(current->info->st_mode & S_ISUID))
		ft_printf("s");
	else if (!(current->info->st_mode & S_IXUSR) &&
										(current->info->st_mode & S_ISUID))
		ft_printf("S");
	else if (current->info->st_mode & S_IXUSR)
		ft_printf("x");
	else
		ft_printf("-");
	print_group_mode(current);
}

void			show_long(t_ls *ls, t_file *current)
{
	print_type_file(current);
	print_user_mode(current);
	print_spaces(ls->max_length_link, current->length_nbr_links);
	ft_printf("%u ", current->info->st_nlink);
	show_user_group(ls, current);
	if (S_ISBLK(current->info->st_mode) || S_ISCHR(current->info->st_mode))
	{
		print_spaces(ls->max_length_major + 1, current->length_major);
		ft_printf("%d, ", (current->info->st_rdev >> 24) & 0xFF);
		if ((current->info->st_rdev & 0xFFFFFF) >= 1000)
			ft_printf("%#010x ", current->info->st_rdev & 0xFFFFFF);
		else
			ft_printf("%3d ", current->info->st_rdev & 0xFFFFFF);
	}
	else
	{
		print_spaces(ls->max_length_size, current->length_nbr_size);
		if (ls->max_length_major > 1)
			print_spaces(ls->max_length_major + 1 + 4, 0);
		ft_printf("%lld ", current->info->st_size);
	}
	print_time(current);
	ft_printf("%s", current->name);
	print_link(current);
	ft_printf("\n");
}
