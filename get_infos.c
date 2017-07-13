/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_infos.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tle-meur <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/01/05 13:56:54 by tle-meur          #+#    #+#             */
/*   Updated: 2016/05/25 16:44:56 by tle-meur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <sys/stat.h>

int		get_path(t_shell *e)
{
	char	*tmp;

	e->path_size = 16;
	ft_memdel((void **)&e->path);
	if (!(e->path = ft_memalloc(e->path_size * sizeof(char))))
		return (0);
	while (e->path_size < 1024 && !getcwd(e->path, e->path_size))
	{
		if (!(tmp = ft_memrealloc(e->path, e->path_size * sizeof(char), \
				e->path_size * 2 * sizeof(char))))
			return (0);
		e->path = tmp;
		e->path_size *= 2;
	}
	if (e->path_size == 1024)
		return (0);
	e->path_size = ft_strlen(e->path);
	return (1);
}

int		is_builtin(t_shell *e)
{
	if (ft_strequ(e->cmd[0], "env"))
		return (ft_env(e, 0, 0));
	else if (ft_strequ(e->cmd[0], "unsetenv"))
		return (ft_unsetenv(e->env, e->cmd + 1));
	else if (ft_strequ(e->cmd[0], "setenv"))
		return (ft_setenv(&e->env, e->cmd + 1, 0, 0));
	else if (ft_strequ(e->cmd[0], "cd"))
		return (ft_cd(e));
	else if (ft_strequ(e->cmd[0], "exit"))
		return (ft_exit(e, 0, 0));
	return (0);
}

int		envchr(char **env, char *ref)
{
	int	ind;
	int	len;

	ind = -1;
	len = ft_strlen(ref);
	while (env[++ind])
		if (!ft_strncmp(env[ind], ref, len) && *(env[ind] + len) == '=')
			return (ind);
	return (-1);
}

void	get_error(t_shell *e)
{
	struct stat	st;
	DIR			*dir;

	if ((dir = opendir(e->cmd[0])))
	{
		closedir(dir);
		ft_printf_fd(2, "minishell: command not found: %s\n", e->cmd[0]);
	}
	else if (!ft_strchr(e->cmd[0], '/'))
		ft_printf_fd(2, "minishell: command not found: %s\n", e->cmd[0]);
	else if (lstat(e->cmd[0], &st) == -1)
		ft_printf_fd(2, "minishell: no such file or directory: %s\n", \
			e->cmd[0]);
	else if (access(e->cmd[0], R_OK | X_OK) == -1)
		ft_printf_fd(2, "minishell: permission denied: %s\n", e->cmd[0]);
}

int		if_exec(t_shell *e, int i, char **v, DIR *dir)
{
	char		*name;

	if ((dir = opendir(e->cmd[0])))
		closedir(dir);
	else if (access(e->cmd[0], X_OK) == 0)
		return (1);
	if ((i = envchr(e->env, "PATH")) == -1
			|| !(v = ft_strsplit(e->env[i] + 5, ':')))
		return ((i == -1) ? 0 : -10);
	i = -1;
	while (v[++i])
	{
		if (!(name = ft_strnew(ft_strlen(v[i]) + 1 + ft_strlen(e->cmd[0]))))
			return (-11);
		my_strcat(name, v[i], e->cmd[0], 0);
		if (access(name, X_OK) == 0)
		{
			ft_tabdel((void ***)&v);
			free(e->cmd[0]);
			return (*(int *)(e->cmd[0] = name) * 0 + 1);
		}
		free(name);
	}
	ft_tabdel((void ***)&v);
	return (0);
}
