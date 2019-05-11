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
			ft_printf("ls: illegal option -- %c\n", flags[i]);
			ft_printf("usage: ls [-R] [file ...]\n");
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

void	open_directory(t_ls *ls, char *directory)
{
	DIR *dir;
	struct dirent entry;

	if ((dir = opendir(directory)) == NULL)
		perror("open_directory()");

	// while ()
}

int	main(int argc, char *argv[])
{
	int i;
	t_ls	*ls;

	if ((ls = (t_ls *)ft_memalloc(sizeof(t_ls))) == NULL)
		ft_error("Can't allocate memory");
	g_fd = 1; // for ft_printf

	ft_printf("endflah = %d\n", ls->end_flags);
	
	i = 1;
	while (i < argc)
	{
		if (argv[i][0] == '-' && ls->end_flags == 0)
			check_flags(argv[i], ls);
		else
			ls->end_flags = 1;
		if (ls->end_flags == 1)
		{
			open_directory(ls, ".");
		}
		i++;
	}

	print_struct_ls(ls);
	return (0);
}
