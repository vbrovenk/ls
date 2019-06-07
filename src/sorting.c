/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sorting.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbrovenk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/17 16:59:12 by vbrovenk          #+#    #+#             */
/*   Updated: 2019/05/17 16:59:13 by vbrovenk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ls.h"

static	void	left_right_split(t_file *source, t_file **left, t_file **right)
{
	t_file	*end;
	t_file	*middle;

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

t_file			*merge_sorted_list(t_file *left, t_file *right)
{
	t_file *result;

	result = NULL;
	if (left == NULL)
		return (right);
	else if (right == NULL)
		return (left);
	if (ft_strcmp(left->name, right->name) > 0)
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

t_file			*merge_reverse_list(t_file *left, t_file *right)
{
	t_file *result;

	result = NULL;
	if (left == NULL)
		return (right);
	else if (right == NULL)
		return (left);
	if (1)
	{
		result = right;
		result->next = merge_reverse_list(left, right->next);
	}
	else
	{
		result = left;
		result->next = merge_reverse_list(left->next, right);
	}
	return (result);
}

t_file			*merge_time_list(t_file *left, t_file *right)
{
	t_file *result;

	result = NULL;
	if (left == NULL)
		return (right);
	else if (right == NULL)
		return (left);
	if (left->info->st_mtimespec.tv_sec < right->info->st_mtimespec.tv_sec)
	{
		result = right;
		result->next = merge_time_list(left, right->next);
	}
	else
	{
		result = left;
		result->next = merge_time_list(left->next, right);
	}
	return (result);
}

void			merge_sort(t_ls *ls, t_file **list_files,
							t_file *(*sorting) (t_file *, t_file *))
{
	t_file *head;
	t_file *left;
	t_file *right;

	head = *list_files;
	if (head == NULL || head->next == NULL)
		return ;
	left_right_split(head, &left, &right);
	merge_sort(ls, &left, sorting);
	merge_sort(ls, &right, sorting);
	*list_files = sorting(left, right);
}
