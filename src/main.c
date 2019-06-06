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
	ft_dprintf(2, "%s\n", message);
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
		else if (flags[i] == 'a')
			ls->flag_a = 1;
		else if (flags[i] == 'r')
			ls->flag_r = 1;
		else if (flags[i] == '1')  // maybe delete this
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

void	print_struct_ls(t_ls *ls)
{
	ft_printf("=========== LS ===========\n");
	ft_printf("ls->flag_R = %d\n", ls->flag_R);
	ft_printf("ls->flag_a = %d\n", ls->flag_a);
	ft_printf("ls->flag_r = %d\n", ls->flag_r);
	ft_printf("ls->flag_l = %d\n", ls->flag_l);
	ft_printf("ls->flag_t = %d\n", ls->flag_t);
	ft_printf("ls->args_files = %d\n", ls->args_files);

	ft_printf("ls->max_group = %d\n", ls->max_length_group);
}


// long long for st_size
void	print_spaces(long long max, long long subtrahend)
{
	int i;

	i = max - subtrahend;
	// ft_printf("i = %d\n", i);
	while (i)
	{
		ft_printf(" ");
		i--;
	}
}

void	show_user_group(t_ls *ls, t_file *current)
{
	struct passwd	*usr;
	struct group	*grp;

	errno = 0;
	usr = getpwuid(current->info->st_uid);
	if (usr == NULL)
	{
		ft_dprintf(2, "getpwuid(): %s\n", strerror(errno));
		ft_printf("%u  ", current->info->st_uid); // in man
		errno = 0;
	}
	else
	{
		ft_printf("%s  ", usr->pw_name);
		print_spaces(ls->max_length_name, current->length_name);
	}
	grp = getgrgid(current->info->st_gid);
	if (grp == NULL)
	{
		ft_dprintf(2, "getgrgid(): %s\n", strerror(errno));
		ft_printf("%u  ", current->info->st_gid); // in man
		errno = 0;
	}
	else
	{
		ft_printf("%s  ", grp->gr_name);
		// ft_printf("group = %d ", ls->max_length_group - current->length_group);
		print_spaces(ls->max_length_group, current->length_group);
	}
}

void	print_time(t_file *current)
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
		ft_printf("%s %2s %s:%s ", split_date[1], split_date[2], split_time[0], split_time[1]);
	ft_split_clear(split_date);
	ft_split_clear(split_time);
}

void	print_link(t_file *current)
{
	char *sym_link;
	int ret;
	
	sym_link = (char *)ft_memalloc(256);
	if (S_ISLNK(current->info->st_mode))
	{
		ret = readlink(current->full_name, sym_link, 255);
		if (ret == -1)
		{
			ft_dprintf(2, "readlink error \'%s\' - %s\n", current->full_name, strerror(errno));
			errno = 0;
		}
		else
			ft_printf(" -> %s", sym_link);
	}
	ft_strdel(&sym_link);
}

void	show_long(t_ls *ls, t_file *current)
{
	if (S_ISBLK(current->info->st_mode))
		ft_printf("b");
	else if (S_ISCHR(current->info->st_mode))
		ft_printf("c");
	else if (S_ISDIR(current->info->st_mode))
		ft_printf("d");
	else if (S_ISLNK(current->info->st_mode))
		ft_printf("l");
	else
		ft_printf("-");

	ft_printf((current->info->st_mode & S_IRUSR) ? "r" : "-");
	ft_printf((current->info->st_mode & S_IWUSR) ? "w" : "-");
	if ((current->info->st_mode & S_IXUSR) && (current->info->st_mode & S_ISUID))
		ft_printf("s");
	else if (!(current->info->st_mode & S_IXUSR) && (current->info->st_mode & S_ISUID))
		ft_printf("S");
	else if (current->info->st_mode & S_IXUSR)
		ft_printf("x");
	else
		ft_printf("-");
	ft_printf((current->info->st_mode & S_IRGRP) ? "r" : "-");
	ft_printf((current->info->st_mode & S_IWGRP) ? "w" : "-");
	// ft_printf((current->info->st_mode & S_IXGRP) ? "x" : "-");
	if ((current->info->st_mode & S_IXGRP) && (current->info->st_mode & S_ISGID))
		ft_printf("s");
	else if (!(current->info->st_mode & S_IXGRP) && (current->info->st_mode & S_ISGID))
		ft_printf("S");
	else if (current->info->st_mode & S_IXGRP)
		ft_printf("x");
	else
		ft_printf("-");


	ft_printf((current->info->st_mode & S_IROTH) ? "r" : "-");
	ft_printf((current->info->st_mode & S_IWOTH) ? "w" : "-");
	ft_printf((current->info->st_mode & S_IXOTH) ? "x  " : "-  ");

	print_spaces(ls->max_length_link, current->length_nbr_links);
	ft_printf("%u ", current->info->st_nlink);

	show_user_group(ls, current);
	if (S_ISBLK(current->info->st_mode) || S_ISCHR(current->info->st_mode))
	{
		print_spaces(ls->max_length_major, current->length_major);
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
			print_spaces(ls->max_length_major + 1, 0);
		ft_printf("%lld ", current->info->st_size);
	}

	print_time(current);
	ft_printf("%s", current->name);
	print_link(current);
	ft_printf("\n");
}

void	show_files(char *directory, t_ls *ls)
{
	t_file *current;

	current = ls->list_files;

	if (ls->single_arg > 1)
	{
		ft_printf("%s:\n", directory);
	}
	else
	{
		ls->single_arg = 2;
	}
	if (ls->flag_l == 1 && current != NULL)
		ft_printf("total %u\n", ls->total_blocks);
	while (current != NULL)
	{
		if (ls->flag_l == 1)
			show_long(ls, current);
		else
			ft_printf("%s\n", current->name);
		current = current->next;
	}
}

char	*join_path(char *part_path, char *current_dir)
{
	char *temp;
	char *full_path;

	full_path = ft_strjoin(part_path, "/");
	temp = full_path;
	full_path = ft_strjoin(full_path, current_dir);
	ft_strdel(&temp);
	return (full_path);
}

void	add_file_to_list(t_ls *ls, struct dirent *entry, char *full_name)
{
	t_file *new;
	t_file *current;

	if ((new = (t_file *)ft_memalloc(sizeof(t_file))) == NULL)
		ft_error("Can't allocate t_file");
	new->name = ft_strdup(entry->d_name);
	new->info = (struct stat*)ft_memalloc(sizeof(struct stat));
	new->full_name = full_name;
	if (lstat(full_name, new->info) < 0)
		ft_dprintf(2, "lstat error \'%s\' - %s\n", full_name, strerror(errno));

	
	new->length_nbr_links = ft_countdigits(new->info->st_nlink);
	if (new->length_nbr_links > ls->max_length_link)
		ls->max_length_link = new->length_nbr_links;
	
	new->length_nbr_size = ft_countdigits(new->info->st_size);
	if (new->length_nbr_size > ls->max_length_size)
		ls->max_length_size = new->length_nbr_size;
	
	new->length_name = ft_strlen(getpwuid(new->info->st_uid)->pw_name);
	if (new->length_name > ls->max_length_name)
		ls->max_length_name = new->length_name;
	
	new->length_group = ft_strlen(getgrgid(new->info->st_gid)->gr_name);
	if (new->length_group > ls->max_length_group)
		ls->max_length_group = new->length_group;

	new->length_major = ft_countdigits((new->info->st_rdev >> 24) & 0xFF);
	if (new->length_major > ls->max_length_major)
		ls->max_length_major = new->length_major;

	ls->total_blocks += new->info->st_blocks;
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

void	add_to_list(t_ls *ls, t_file **head, char *name, char *full_name)
{
	t_file *current;
	t_file *new;

	current = *head;
	new = (t_file *)ft_memalloc(sizeof(t_file));
	new->name = ft_strdup(name);
	new->info = (struct stat*)ft_memalloc(sizeof(struct stat));
	new->full_name = full_name;

	if (lstat(full_name, new->info) < 0)
		ft_dprintf(2, "lstat error \'%s\' - %s\n", full_name, strerror(errno));

	new->length_nbr_links = ft_countdigits(new->info->st_nlink);
	// ft_printf("length = %d | value = %d | name = %s\n", new->length_nbr_links, new->info->st_nlink, name);
	if (new->length_nbr_links > ls->max_length_link)
		ls->max_length_link = new->length_nbr_links;

	new->length_nbr_size = ft_countdigits(new->info->st_size);
	if (new->length_nbr_size > ls->max_length_size)
		ls->max_length_size = new->length_nbr_size;
	
	new->length_name = ft_strlen(getpwuid(new->info->st_uid)->pw_name);
	if (new->length_name > ls->max_length_name)
		ls->max_length_name = new->length_name;
	
	new->length_group = ft_strlen(getgrgid(new->info->st_gid)->gr_name);
	if (new->length_group > ls->max_length_group)
		ls->max_length_group = new->length_group;
	
	new->length_major = ft_countdigits((new->info->st_rdev >> 24) & 0xFF);
	if (new->length_major > ls->max_length_major)
		ls->max_length_major = new->length_major;

	ls->total_blocks += new->info->st_blocks;
	if (current == NULL)
		*head = new;
	else
	{
		while (current->next != NULL)
			current = current->next;
		current->next = new;
	}
}


void	regular_sort_files(t_ls *ls)
{
	t_file *current_file;
	t_file *after_file;
	char	*temp;

	current_file = ls->list_files;
	while (current_file->next != NULL)
	{
		after_file = current_file->next;
		while (after_file != NULL)
		{
			if (ft_strcmp(current_file->name, after_file->name) > 0)
			{
				temp = current_file->name;
				current_file->name = after_file->name;
				after_file->name = temp;
			}
			after_file = after_file->next;
		}
		current_file = current_file->next;
	}
}

t_file	*sort_and_show(t_ls *ls, DIR *dir, char *dir_name, t_file **dirs)
{
	choose_sort(ls, &ls->list_files);
	choose_sort(ls, dirs);
	closedir(dir);
	show_files(dir_name, ls);
	// ft_printf("ls->max_length_link = %d\n", ls->max_length_link);
	ls->max_length_link = 0;
	ls->max_length_name = 0;
	ls->max_length_group = 0;
	ls->max_length_size = 0;
	ls->max_length_major = 0;
	ls->total_blocks = 0;
	ls->list_files = free_list_files(ls->list_files);
	return (*dirs);
}

void	adding_file(t_ls *ls, struct dirent *entry, t_file **dirs, char *path_to_file)
{
	char *full_name;

	if (entry->d_type == DT_DIR && ft_strequ(entry->d_name, ".") == 0
								&& ft_strequ(entry->d_name, "..") == 0)
	{
		if (ls->flag_a == 1 || entry->d_name[0] != '.')
		{
			full_name = join_path(path_to_file, entry->d_name);
			add_to_list(ls, dirs, entry->d_name, full_name);
		}
	}
	// all content add to list of current dir
	if (ls->flag_a == 1 || entry->d_name[0] != '.')
	{
		full_name = join_path(path_to_file, entry->d_name);
		add_file_to_list(ls, entry, full_name);		
	}
}

void	open_directory(t_ls *ls, char *full_name, char *dir_name)
{
	DIR				*dir;
	struct dirent	*entry;
	t_file			*nested_dirs;
	t_file			*current_dir;
	char			*full_path;

	dir = opendir(full_name);
	nested_dirs = NULL;
	if (errno == EACCES)
	{
		ft_printf("%s:\n", full_name);
		ft_dprintf(2, "ft_ls: %s : %s\n", dir_name, strerror(errno));
		errno = 0;
		return ;
	}
	while ((entry = readdir(dir)) != NULL)
		adding_file(ls, entry, &nested_dirs, full_name);
	current_dir = sort_and_show(ls, dir, full_name, &nested_dirs);
	if (ls->flag_R == 1)
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


void	print_args(t_ls *ls, int argc, char *argv[]) // not use
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

void	sort_args(t_ls *ls, int argc, char *argv[])
{
	int i;
	DIR *dir;

	struct stat *check_lnk;

	i = ls->args_files;
	ls->single_arg = argc - ls->args_files;
	while (i < argc)
	{
		if ((dir = opendir(argv[i])) == NULL)
		{
			check_lnk = (struct stat *)ft_memalloc(sizeof(struct stat));
			if (lstat(argv[i], check_lnk) == 0 && S_ISLNK(check_lnk->st_mode))
				add_to_list(ls, &ls->non_dirs, argv[i], ft_strdup(argv[i]));
			else if (errno == ENOENT)
				ft_dprintf(2, "ft_ls: %s: %s\n", argv[i], strerror(errno));
			else if (errno == ENOTDIR)
				add_to_list(ls, &ls->non_dirs, argv[i], ft_strdup(argv[i]));
			else if (errno == EACCES)
				ft_dprintf(2, "ft_ls: %s: %s\n", argv[i], strerror(errno));
			errno = 0;
			free(check_lnk);
		}
		else
		{
			add_to_list(ls, &ls->dirs, argv[i], ft_strdup(argv[i]));
			ls->max_length_link = 0;
			ls->max_length_name = 0;
			ls->max_length_group = 0;
			ls->max_length_size = 0;
			ls->max_length_major = 0;
			ls->total_blocks = 0;
			closedir(dir);
		}
		i++;
	}
	choose_sort(ls, &ls->non_dirs);
	choose_sort(ls, &ls->dirs);
}


// 104 tests passed
int	main(int argc, char *argv[])
{
	int i;
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
	if (ls->args_files != 0)
	{
		sort_args(ls, argc, argv);
		i = ls->args_files;
		t_file *current = ls->non_dirs;
		while (current)
		{
			if (ls->flag_l == 1)
				show_long(ls, current);
			else
				ft_printf("%s\n", current->name);
			current = current->next;
		}
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
	{
		open_directory(ls, ".", ".");
	}
	
	// print_struct_ls(ls);

	// system("leaks ft_ls");
	return (0);
}
