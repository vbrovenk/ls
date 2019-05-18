#!/bin/sh

ls $@ > diff_ls
./ft_ls $@ > diff_my
diff -y diff_ls diff_my
#rm diff_ls diff_my