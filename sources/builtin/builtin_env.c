/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cscache <cscache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 16:42:47 by barmarti          #+#    #+#             */
/*   Updated: 2025/09/16 12:46:32 by cscache          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libft/libft.h"
#include "../../includes/minishell.h"

int	builtin_env(t_env *env, t_cmd *cmd)
{
	t_env	*tmp;

	if (!env)
	{
		ft_putendl_fd("minishell: env: missing envp", 2);
		return (EXIT_CMD_NOT_FOUND);
	}
	if (cmd && cmd->args[1])
	{
		ft_putendl_fd("minishell: env: too many arguments", 2);
		return (EXIT_FAILURE);
	}
	tmp = env;
	while (tmp)
	{
		if (ft_printf("%s=%s\n", tmp->key, tmp->value) < 0)
			return (EXIT_FAILURE);
		tmp = tmp->next;
	}
	return (EXIT_SUCCESS);
}
