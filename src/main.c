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

t_file	*free_list_files(t_file *list_files)
{
	t_file *before;

	while (list_files != NULL)
	{
		free(list_files->entry);
		before = list_files;
		list_files = list_files->next;
		free(before);	
	}
	return (NULL);
}

void	print_struct_ls(t_ls *ls)
{
	ft_printf("=========== LS ===========\n");
	ft_printf("ls->flag_R = %d\n", ls->flag_R);
	ft_printf("ls->args_files = %d\n", ls->args_files);
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

void	regular_sort_files(t_ls *ls)
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

void	front_back_split(t_file *source, t_file **left, t_file **right)
{
	t_file *end;
	t_file *middle;

	middle = source;
	end = source->next;
	while (end != NULL)
	{
		end = end->next;
		if (end != NULL)
		{
			middle = middle->next;
			end = end->next;
		}
	}
	*left = source;
	*right = middle->next;
	middle->next = NULL;
}

t_file	*merge_sorted_list(t_file *left, t_file *right)
{
	t_file *result;

	result = NULL;
	if (left == NULL)
		return (right);
	else if (right == NULL)
		return (left);
	if (ft_strcmp(left->entry->d_name, right->entry->d_name) > 0)
	{
		result = right;
		result->next = merge_sorted_list(left, right->next);
	}
	else
	{
		result = left;
		result->next = merge_sorted_list(left->next, right);
	}
	return (result);
}

void	merge_sort(t_file **list_files)
{
	t_file *head;
	t_file *left;
	t_file *right;

	head = *list_files;
	if (head == NULL || head->next == NULL)
		return ;
	front_back_split(head, &left, &right);
	// recursive sort sublists
	merge_sort(&left);
	merge_sort(&right);

	*list_files = merge_sorted_list(left, right);
}


void	open_directory(t_ls *ls, char *directory)
{
	DIR *dir;
	struct dirent *entry;

	if ((dir = opendir(directory)) == NULL)
	{
		ft_printf("ft_ls: %s: %s\n", directory, strerror(errno));
		return ;
	}

	while ((entry = readdir(dir)) != NULL)
	{
		add_file_to_list(ls, entry);
	}

	// regular_sort_files(ls);
	merge_sort(&ls->list_files);

	closedir(dir);
	show_files(ls);
	ls->list_files = free_list_files(ls->list_files);
}


void	print_args(t_ls *ls, int argc, char *argv[])
{
	int i;

	i = ls->args_files;
	while (i < argc)
	{
		ft_printf("%s ", argv[i]);
		i++;
	}
	ft_printf("\n");
}

void	add_to_list(t_args **head, char *name)
{
	t_args *current;
	t_args *new;

	current = *head;
	new = (t_args *)ft_memalloc(sizeof(t_args));
	new->name = ft_strdup(name);
	if (current == NULL)
	{
		*head = new;
	}
	else
	{
		while (current->next != NULL)
			current = current->next;
		current->next = new;
	}
}

void	sort_args(t_ls *ls, int argc, char *argv[])
{
	int i;
	DIR *dir;

	i = ls->args_files;
	while (i < argc)
	{
		if ((dir = opendir(argv[i])) == NULL)
		{
			if (errno == ENOENT)
				ft_printf("ft_ls: %s: %s\n", argv[i], strerror(errno));
			else if (errno == ENOTDIR)
				add_to_list(&ls->non_dirs, argv[i]);
		}
		else
		{
			add_to_list(&ls->dirs, argv[i]);
		}
		i++;
	}
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
		else if (ls->args_files == 0)
			ls->args_files = i;
		i++;
	}
	if (ls->args_files != 0)
	{
		sort_args(ls, argc, argv);
		i = ls->args_files;
		t_args *current = ls->non_dirs;
		while (current)
		{
			ft_printf("%s\n", current->name);
			current = current->next;
		}
		current = ls->dirs;
		while (current != NULL)
		{
			open_directory(ls, current->name);
			current = current->next;

		}
		// while (i < argc)
		// {
		// 	open_directory(ls, argv[i]);
		// 	i++;
		// }

	}
	else
	{
		open_directory(ls, ".");
	}
	
	// print_struct_ls(ls);

	// system("leaks ft_ls");
	return (0);
}
