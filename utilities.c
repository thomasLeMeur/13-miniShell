/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utilities.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tle-meur <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/01/05 13:56:54 by tle-meur          #+#    #+#             */
/*   Updated: 2016/05/25 15:18:34 by tle-meur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <sys/wait.h>

int		print_env(char **env)
{
	int	i;

	i = -1;
	while (env[++i])
		ft_putendl(env[i]);
	return (1);
}

int		split_line(t_shell *e)
{
	ft_tabdel((void ***)&e->cmd);
	if (!(e->cmd = ft_strsplit(e->line, ' ')))
		return (0);
	return (1);
}

void	ft_fork(t_shell *e, char *prog, char **argv, char **envp)
{
	int	pid;

	if ((pid = fork()) == -1)
	{
		ft_putendl_fd("Didn't success to fork.", 2);
		ft_exit(e, e->ret, 0);
	}
	else if (pid == 0)
	{
		if (execve(prog, argv, envp) == -1)
			ft_exit(e, 2, 1);
	}
	else
		waitpid(pid, NULL, 0);
}

int		init_env(t_shell *e)
{
	int			len;
	extern char	**environ;

	len = 0;
	e->cmd = NULL;
	e->env = NULL;
	e->line = NULL;
	e->path = NULL;
	e->env_tmp = NULL;
	if (!get_path(e) && (e->ret = -2))
		return (0);
	while (environ[len])
		len++;
	if (!(e->env = ft_memalloc((len + 1) * sizeof(char *))))
		return ((e->ret = -3) * 0);
	while (--len >= 0)
		if (!(e->env[len] = ft_strdup(environ[len])))
		{
			ft_tabdel((void ***)&e->env);
			e->ret = -4;
			return (0);
		}
	return (1);
}

void	my_strcat(char *buf1, char *buf2, char *buf3, int style)
{
	int	buf2_s;
	int	buf3_s;

	buf2_s = ft_strlen(buf2);
	buf3_s = ft_strlen(buf3);
	ft_memcpy(buf1, buf2, buf2_s);
	ft_memcpy(buf1 + buf2_s, (style) ? "=" : "/", 1);
	ft_memcpy(buf1 + buf2_s + 1, buf3, buf3_s + (style));
}
