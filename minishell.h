/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tle-meur <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/01/05 13:56:19 by tle-meur          #+#    #+#             */
/*   Updated: 2016/05/25 16:05:47 by tle-meur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <dirent.h>
# include "libft.h"

typedef struct	s_shell
{
	int		ret;
	char	**cmd;
	char	**env;
	char	**env_tmp;
	char	*line;
	char	*path;
	int		path_size;
}				t_shell;

/*
**	main.c
*/
int				ft_env_intern_loop(t_shell *e, int *r, int u, int i);

/*
**	utilities.c
*/
int				init_env(t_shell *e);
int				print_env(char **env);
int				split_line(t_shell *e);
void			my_strcat(char *buf1, char *buf2, char *buf3, int style);
void			ft_fork(t_shell *e, char *prog, char **argv, char **envp);

/*
**	builtins.c
*/
int				ft_cd(t_shell *e);
int				ft_env(t_shell *e, int i, int r);
int				ft_unsetenv(char **env, char **args);
int				ft_exit(t_shell *e, int ret, int cmt);
int				ft_setenv(char ***env, char **args, int i, int ind);

/*
**	get_infos.c
*/
int				get_path(t_shell *e);
void			get_error(t_shell *e);
int				is_builtin(t_shell *e);
int				envchr(char **env, char *ref);
int				if_exec(t_shell *e, int i, char **v, DIR *dir);

#endif
