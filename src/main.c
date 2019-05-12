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

void	ft_error(char *message)
{
	g_fd = 2;
	ft_printf("%s : %s\n", message, strerror(errno));
	exit(1);
}

void	check_flags(char *flags, t_ls *ls)
{
	int i;

	i = 1;
	while (flags[i])
	{
		if (flags[i] == 'R')
			ls->flag_R = 1;
		else
		{
			ft_printf("ft_ls: illegal option -- %c\n", flags[i]);
			ft_printf("usage: ./ft_ls [-R] [file ...]\n");
			exit(1);
		}
		i++;
	}
}

void	print_struct_ls(t_ls *ls)
{
	ft_printf("=========== LS ===========\n");
	ft_printf("ls->flag_R = %d\n", ls->flag_R);
}

void	show_files(t_ls *ls)
{
	t_file *current;

	ft_printf("========= LIST OF FILES ===========\n");
	current = ls->list_files;
	while (current != NULL)
	{
		ft_printf("current->d_name = %s\n", current->entry->d_name);
		current = current->next;
	}
}

void	add_file_to_list(t_ls *ls, struct dirent *entry)
{
	t_file *new;
	t_file *current;

	if ((new = (t_file *)ft_memalloc(sizeof(t_file))) == NULL)
		ft_error("Can't allocate t_file");
	new->entry = (struct dirent *)malloc(sizeof(struct dirent));
	ft_memcpy(new->entry, entry, sizeof(struct dirent));
	current = ls->list_files;
	if (ls->list_files == NULL)
	{
		ls->list_files = new;
		ls->last_file = new;
	}
	else
	{
		ls->last_file->next = new;
		ls->last_file = ls->last_file->next;
	}

}

void	sort_files(t_ls *ls)
{
	t_file *current_file;
	t_file *after_file;
	struct dirent *temp;

	current_file = ls->list_files;
	while (current_file->next != NULL)
	{
		after_file = current_file->next;
		while (after_file != NULL)
		{
			if (ft_strcmp(current_file->entry->d_name,
							after_file->entry->d_name) > 0)
			{
				temp = current_file->entry;
				current_file->entry = after_file->entry;
				after_file->entry = temp;
			}
			after_file = after_file->next;
		}
		current_file = current_file->next;
	}
}

void	open_directory(t_ls *ls, char *directory)
{
	DIR *dir;
	struct dirent *entry;

	if ((dir = opendir(directory)) == NULL)
		perror("open_directory()");

	while ((entry = readdir(dir)) != NULL)
	{
		add_file_to_list(ls, entry);
		// show_files(ls);
	}

	sort_files(ls);
	// ft_printf("NORm\n");

	show_files(ls);
}

int	main(int argc, char *argv[])
{
	int i;
	t_ls	*ls;

	if ((ls = (t_ls *)ft_memalloc(sizeof(t_ls))) == NULL)
		ft_error("Can't allocate memory");
	g_fd = 1; // for ft_printf
	
	i = 1;
	while (i < argc)
	{
		if (argv[i][0] == '-' && ls->args_files == 0)
			check_flags(argv[i], ls);
		else
			ls->args_files = i;
		i++;
	}
	if (ls->args_files != 0)
	{
		i = ls->args_files;
		while (i < argc)
		{
			open_directory(ls, argv[i]);
			i++;
		}
	}
	else
	{
		open_directory(ls, ".");
	}
		
	// print_struct_ls(ls);
	return (0);
}
