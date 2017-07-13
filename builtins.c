/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tle-meur <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/01/05 13:56:54 by tle-meur          #+#    #+#             */
/*   Updated: 2016/05/25 16:16:31 by tle-meur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_exit(t_shell *e, int ret, int cmt)
{
	ft_tabdel((void ***)&e->cmd);
	ft_tabdel((void ***)&e->env);
	ft_memdel((void **)&e->line);
	ft_memdel((void **)&e->path);
	ft_tabdel((void ***)&e->env_tmp);
	if (!ret && cmt)
		ft_putendl("^D");
	else if (cmt)
		ft_putendl_fd("An error occured.", 2);
	exit(ret);
	return (1);
}

int	ft_cd(t_shell *e)
{
	int		ret;
	char	*tmp;

	if (e->cmd[1])
		tmp = e->cmd[1];
	else
	{
		if ((ret = envchr(e->env, "HOME")) == -1)
		{
			ft_putendl_fd("minishell: cd: HOME not set", 2);
			return (1);
		}
		else
			tmp = e->env[ret] + 5;
	}
	if (chdir(tmp) == -1)
		return (ft_printf_fd(2, "minishell: cd: %s failed to open\n", tmp));
	tmp = e->path;
	e->path = NULL;
	if (!get_path(e))
	{
		ft_memdel((void **)&e->path);
		e->path = tmp;
	}
	return (1);
}

int	ft_setenv(char ***env, char **args, int i, int ind)
{
	char	*val;
	char	*tmp;
	char	**temp;

	(!args[0]) ? print_env(*env) : (i = -2);
	if (args[0])
		while (((i += 2) || 1) && (i == 0 || args[i - 1]) && args[i])
		{
			val = (args[i + 1]) ? args[i + 1] : "";
			if (!(tmp = ft_strnew(ft_strlen(args[i]) + 1 + ft_strlen(val))))
				return (-8);
			my_strcat(tmp, args[i], val, 1);
			if ((ind = envchr(*env, args[i])) == -1)
			{
				while ((*env)[++ind])
					;
				if (!(temp = ft_memrealloc(*env, (ind + 1) * sizeof(char *), \
						(ind + 2) * sizeof(char *))))
					return (-9);
				*env = temp;
			}
			ft_memdel((void **)&(*env)[ind]);
			(*env)[ind] = tmp;
		}
	return (1);
}

int	ft_unsetenv(char **env, char **args)
{
	int		i;
	int		ind;

	if (!args[0])
		print_env(env);
	else if ((i = -1))
		while (args[++i])
			if ((ind = envchr(env, args[i])) != -1)
			{
				ft_memdel((void **)&env[ind]);
				while (env[++ind])
					FT_SWAP(env[ind - 1], env[ind], char *);
			}
	return (1);
}

int	ft_env(t_shell *e, int i, int r)
{
	int		len;

	len = -1;
	while (e->env[++len])
		;
	if (!(e->env_tmp = ft_memalloc((len + 1) * sizeof(char *))))
		return (-15);
	while (--len >= 0)
		if (!(e->env_tmp[len] = ft_strdup(e->env[len])))
			return (-14);
	if ((len = ft_env_intern_loop(e, &r, 0, 0)) < 0)
		return (len);
	if (!(i = 0) && len == 2 && e->cmd[r] == NULL)
		r++;
	ft_memdel((void **)&e->cmd[0]);
	while (e->cmd[r])
	{
		FT_SWAP(e->cmd[r], e->cmd[i], char *);
		(void)((r += 1) + (i += 1));
	}
	e->cmd[i] = NULL;
	return ((e->cmd[0]) ? 2 : print_env(e->env_tmp));
}
