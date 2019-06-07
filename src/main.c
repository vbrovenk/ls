/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbrovenk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/11 14:01:44 by vbrovenk          #+#    #+#             */
/*   Updated: 2019/05/11 14:01:46 by vbrovenk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ls.h"

static	void	check_flags(char *flags, t_ls *ls)
{
	int i;

	i = 1;
	while (flags[i])
	{
		if (flags[i] == 'R')
			ls->recursion = 1;
		else if (flags[i] == 'a')
			ls->flag_a = 1;
		else if (flags[i] == 'r')
			ls->flag_r = 1;
		else if (flags[i] == '1')
			;
		else if (flags[i] == 'l')
			ls->flag_l = 1;
		else if (flags[i] == 't')
			ls->flag_t = 1;
		else
		{
			ft_dprintf(2, "ft_ls: illegal option -- %c\n", flags[i]);
			ft_dprintf(2, "usage: ./ft_ls [-Ralrt] [file ...]\n");
			exit(1);
		}
		i++;
	}
}

static	void	determine_errors(t_ls *ls, char *argv[],
											struct stat *check_lnk, int i)
{
	if (lstat(argv[i], check_lnk) == 0 && S_ISLNK(check_lnk->st_mode))
		add_to_list(&ls->non_dirs, argv[i], ft_strdup(argv[i]));
	else if (errno == ENOENT)
		ft_dprintf(2, "ft_ls: %s: %s\n", argv[i], strerror(errno));
	else if (errno == ENOTDIR)
		add_to_list(&ls->non_dirs, argv[i], ft_strdup(argv[i]));
	else if (errno == EACCES)
		ft_dprintf(2, "ft_ls: %s: %s\n", argv[i], strerror(errno));
	errno = 0;
}

static	void	sort_args(t_ls *ls, int argc, char *argv[])
{
	int			i;
	DIR			*dir;
	struct stat	*check_lnk;

	i = ls->args_files;
	ls->single_arg = argc - ls->args_files;
	while (i < argc)
	{
		check_lnk = (struct stat *)ft_memalloc(sizeof(struct stat));
		if (ls->flag_l == 1 && lstat(argv[i], check_lnk) == 0 &&
												S_ISLNK(check_lnk->st_mode))
			add_to_list(&ls->non_dirs, argv[i], ft_strdup(argv[i]));
		else if ((dir = opendir(argv[i])) == NULL)
			determine_errors(ls, argv, check_lnk, i);
		else
		{
			add_to_list(&ls->dirs, argv[i], ft_strdup(argv[i]));
			closedir(dir);
		}
		free(check_lnk);
		i++;
	}
	choose_sort(ls, &ls->non_dirs);
	choose_sort(ls, &ls->dirs);
}

static	void	processing_files(t_ls *ls, int argc, char *argv[])
{
	int		i;
	t_file	*current;

	if (ls->args_files != 0)
	{
		sort_args(ls, argc, argv);
		i = ls->args_files;
		current = ls->non_dirs;
		show_files(NULL, ls, ls->non_dirs, 0);
		if (ls->non_dirs && ls->dirs)
			ft_printf("\n");
		current = ls->dirs;
		while (current != NULL)
		{
			open_directory(ls, current->name, current->name);
			if (current->next != NULL)
				ft_printf("\n");
			current = current->next;
		}
	}
	else
		open_directory(ls, ".", ".");
}

int				main(int argc, char *argv[])
{
	int		i;
	t_ls	*ls;

	if ((ls = (t_ls *)ft_memalloc(sizeof(t_ls))) == NULL)
		ft_error("Can't allocate memory");
	i = 1;
	while (i < argc)
	{
		if (argv[i][0] == '-' && argv[i][1] == '\0' && ls->args_files == 0)
			ls->args_files = i;
		else if (argv[i][0] == '-' && ls->args_files == 0)
			check_flags(argv[i], ls);
		else if (ls->args_files == 0)
			ls->args_files = i;
		i++;
	}
	processing_files(ls, argc, argv);
	return (0);
}
