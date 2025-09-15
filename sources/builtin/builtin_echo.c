/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cscache <cscache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 16:47:11 by barmarti          #+#    #+#             */
/*   Updated: 2025/09/15 14:39:09 by cscache          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libft/libft.h"
#include "../../includes/minishell.h"

static bool	is_n_option(char *arg)
{
	int	i;

	i = 2;
	if (!arg)
		return (0);
	if (ft_strncmp(arg, "-n", 2) == 0)
	{
		while (arg[i])
		{
			if (arg[i] != 'n')
				return (false);
			i++;
		}
		return (true);
	}
	return (false);
}

static bool	process_n_options(char **args, int *i)
{
	bool	option;

	option = false;
	while (args && args[*i] && is_n_option(args[*i]))
	{
		option = true;
		(*i)++;
	}
	return (option);
}

int	builtin_echo(char **args)
{
	bool	option;
	bool	first;
	int		i;

	int j = 0;
	printf("=============ARGS=============\n");
	while (args[j])
	{
		printf("arg[%d] = [%s]\n", j, args[j]);
		j++;
	}
	printf("=============================\n");
	i = 1;
	first = true;
	option = process_n_options(args, &i);
	while (args && args[i])
	{
		if (!first)
			ft_printf(" ");
		ft_printf("%s",args[i]);
		first = false;
		i++;
	}
	if (!option)
		ft_printf("\n");
	return (EXIT_SUCCESS);
}
