/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export_utils_2.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cscache <cscache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 15:39:16 by barmarti          #+#    #+#             */
/*   Updated: 2025/09/16 17:45:28 by cscache          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libft/libft.h"
#include "../../includes/minishell.h"

t_env	*create_new_env_node(t_env *new, char *input, char *key)
{
	if (!input || !key)
		return (NULL);
	new->key = key;
	new->value = get_input_value(input);
	return (new);
}

int	print_env_export(t_env *env)
{
	t_env	*tmp;

	if (!env)
	{
		ft_putendl_fd("minishell: env: missing envp", 2);
		return (EXIT_CMD_NOT_FOUND);
	}
	tmp = env;
	while (tmp)
	{
		if (tmp->value)
		{
			if (ft_printf("export %s=\"%s\"\n", tmp->key, tmp->value) < 0)
				return (EXIT_FAILURE);
		}
		else
		{
			if (ft_printf("export %s\n", tmp->key) < 0)
				return (EXIT_FAILURE);
		}
		tmp = tmp->next;
	}
	return (EXIT_SUCCESS);
}
