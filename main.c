/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tle-meur <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/01/05 13:56:54 by tle-meur          #+#    #+#             */
/*   Updated: 2016/05/25 16:25:01 by tle-meur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	my_add_elem(t_shell *e, int i, int *r, int *u)
{
	char	**tmp;

	if (*u && (i = 1))
	{
		if (!(tmp = ft_memalloc(2 * sizeof(char *))))
			return (-11);
		tmp[0] = e->cmd[*r];
		(void)(ft_unsetenv(e->env_tmp, tmp) + (*u *= 0));
		ft_memdel((void **)&tmp);
	}
	else if (ft_strchr(e->cmd[*r], '=') && (i = 1))
	{
		if (!(tmp = ft_memalloc(3 * sizeof(char *)))
			|| !(tmp[0] = ft_strnew(ft_strchr(e->cmd[*r], '=') - e->cmd[*r])))
		{
			ft_memdel((void **)tmp);
			return ((tmp) ? -13 : -12);
		}
		ft_memcpy(tmp[0], e->cmd[*r], ft_strchr(e->cmd[*r], '=') - e->cmd[*r]);
		tmp[1] = ft_strchr(e->cmd[*r], '=') + 1;
		ft_setenv(&e->env_tmp, tmp, 0, 0);
		ft_memdel((void **)&tmp);
	}
	return (i);
}

int			ft_env_intern_loop(t_shell *e, int *r, int u, int i)
{
	int		len;

	len = 0;
	while (e->cmd[++*r])
	{
		if (*e->cmd[*r] == '-' && (!e->cmd[*r][1] || ft_strchr(e->cmd[*r], 'i'))
				&& (i = (u *= 0) + (len *= 0) + 1))
			while (e->env_tmp[len])
				ft_memdel((void **)&e->env_tmp[len++]);
		else if (!i && *e->cmd[*r] == '-' && ft_strchr(e->cmd[*r], 'u'))
			u = 1;
		else if ((len = my_add_elem(e, 0, r, &u)))
		{
			if (len < 0)
				return (len);
		}
		else if ((u = 2))
			break ;
		ft_memdel((void **)&e->cmd[*r]);
	}
	return (u);
}

static void	main_intern_loop(t_shell *e)
{
	while (1)
	{
		if (!e->cmd[0])
			;
		else if ((e->ret = is_builtin(e)))
		{
			(e->ret < 0) ? ft_exit(e, e->ret, 1) : 0;
			if (e->ret == 2)
				continue ;
		}
		else if ((e->ret = if_exec(e, 0, NULL, NULL)))
		{
			(e->ret < 0) ? ft_exit(e, e->ret, 1) : 0;
			ft_fork(e, e->cmd[0], e->cmd, (e->env_tmp) ? e->env_tmp : e->env);
		}
		else
			get_error(e);
		ft_tabdel((void ***)&e->env_tmp);
		(void)(write(1, e->path, e->path_size) + write(1, " $> ", 4));
		break ;
	}
}

int			main(void)
{
	t_shell	e;

	(!init_env(&e)) ? ft_exit(&e, e.ret, 0) : 0;
	(void)(write(1, e.path, e.path_size) + write(1, " $> ", 4));
	while ((e.ret = ft_gnl(0, &e.line)) > 0)
	{
		(e.ret != 2) ? ft_putstr("\n") : 0;
		if (!split_line(&e))
			ft_exit(&e, -5, 1);
		main_intern_loop(&e);
	}
	return (ft_exit(&e, e.ret, 1));
}
