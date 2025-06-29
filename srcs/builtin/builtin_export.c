/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thi-huon <thi-huon@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/08 15:50:07 by thi-huon          #+#    #+#             */
/*   Updated: 2025/06/12 20:51:31 by thi-huon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

static int	export_handle_one(t_shell *mshell, const char *arg)
{
	char	*equal;
	char	*plus_equal;

	if (!arg)
		return (0);
	if (arg[0] == '-')
	{
		ft_printf_fd(2, "minishell: export: `%s`: invalid option\n", arg);
		return (2);
	}
	plus_equal = ft_strnstr(arg, "+=", ft_strlen(arg));
	if (plus_equal != NULL)
		return (handle_plus_equal_case(mshell, arg, plus_equal));
	equal = ft_strchr(arg, '=');
	if (equal != NULL)
		return (handle_equal_case(mshell, arg, equal));
	else if (!export_is_valid_key(arg))
	{
		ft_printf_fd(2, "minishell: export: `%s`: not a valid identifier\n",
			arg);
		return (1);
	}
	return (0);
}

static void	move_underscore_last(char **envp, int i, int j, int len)
{
	char	*temp;

	if (!envp)
		return ;
	while (envp[len])
		len++;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], "_=", 2) == 0)
		{
			if (i == len - 1)
				return ;
			temp = envp[i];
			j = i;
			while (j < len - 1)
			{
				envp[j] = envp[j + 1];
				j++;
			}
			envp[len - 1] = temp;
			return ;
		}
		i++;
	}
}

static int	handle_export_arguments(t_shell *mshell, char **token, int argc)
{
	int	i;
	int	code;
	int	result;

	i = 1;
	code = 0;
	while (i < argc)
	{
		result = export_handle_one(mshell, token[i]);
		if (result > code)
			code = result;
		i++;
	}
	move_underscore_last(mshell->envp, 0, 0, 0);
	return (code);
}

static int	export_standalone(char **envp)
{
	size_t	count;
	size_t	i;
	char	**copy;

	count = 0;
	i = 0;
	copy = copy_and_sort_envp(envp, &count);
	if (!copy)
		return (1);
	while (i < count)
	{
		if (copy[i][0] != '_' || (copy[i][1] != '\0' && copy[i][1] != '='))
			export_print(copy[i]);
		i++;
	}
	free_split(copy);
	return (0);
}

int	builtin_export(t_shell *mshell, char **token)
{
	int	argc;

	if (!mshell || !token || !token[0])
	{
		ft_printf_fd(2, "minishell: export: invalid input\n");
		return (1);
	}
	argc = 0;
	while (token[argc])
	{
		argc++;
	}
	if (argc == 1)
		return (export_standalone(mshell->envp));
	else
		return (handle_export_arguments(mshell, token, argc));
}
