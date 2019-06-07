/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   work_with_list.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbrovenk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/07 13:11:18 by vbrovenk          #+#    #+#             */
/*   Updated: 2019/06/07 13:11:20 by vbrovenk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ls.h"

void	ft_error(char *message)
{
	ft_dprintf(2, "%s\n", message);
	exit(1);
}

void	add_file_to_list(t_ls *ls, struct dirent *entry, char *full_name)
{
	t_file	*new;
	t_file	*current;

	if ((new = (t_file *)ft_memalloc(sizeof(t_file))) == NULL)
		ft_error("Can't allocate t_file");
	new->name = ft_strdup(entry->d_name);
	new->info = (struct stat*)ft_memalloc(sizeof(struct stat));
	new->full_name = full_name;
	if (lstat(full_name, new->info) < 0)
		ft_dprintf(2, "lstat error \'%s\' - %s\n", full_name, strerror(errno));
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

void	add_to_list(t_file **head, char *name, char *full_name)
{
	t_file	*current;
	t_file	*new;

	current = *head;
	new = (t_file *)ft_memalloc(sizeof(t_file));
	new->name = ft_strdup(name);
	new->info = (struct stat*)ft_memalloc(sizeof(struct stat));
	new->full_name = full_name;
	if (lstat(full_name, new->info) < 0)
		ft_dprintf(2, "lstat error \'%s\' - %s\n", full_name, strerror(errno));
	if (current == NULL)
		*head = new;
	else
	{
		while (current->next != NULL)
			current = current->next;
		current->next = new;
	}
}

t_file	*free_list_files(t_file *list_files)
{
	t_file *before;

	while (list_files != NULL)
	{
		free(list_files->name);
		free(list_files->info);
		ft_strdel(&list_files->full_name);
		before = list_files;
		list_files = list_files->next;
		free(before);
	}
	return (NULL);
}
