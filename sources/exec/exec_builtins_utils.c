/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtins_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cscache <cscache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 11:56:08 by cscache           #+#    #+#             */
/*   Updated: 2025/09/15 11:10:33 by cscache          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	is_a_builtin(char *name)
{
	if (!name)
		return (0);
	else if (!ft_strcmp(name, "env"))
		return (1);
	else if (!ft_strcmp(name, "pwd"))
		return (1);
	else if (!ft_strcmp(name, "cd"))
		return (1);
	else if (!ft_strcmp(name, "unset"))
		return (1);
	else if (!ft_strcmp(name, "export"))
		return (1);
	else if (!ft_strcmp(name, "echo"))
		return (1);
	else if (!ft_strcmp(name, "exit"))
		return (1);
	else
		return (0);
}

bool	is_parent_builtin(char *name)
{
	if (!name)
		return (false);
	else if (!ft_strcmp(name, "cd"))
		return (true);
	else if (!ft_strcmp(name, "exit"))
		return (true);
	else if (!ft_strcmp(name, "export"))
		return (true);
	else if (!ft_strcmp(name, "unset"))
		return (true);
	return (false);
}

int	execute_builtins(t_cmd *cmd, t_shell *shell)
{
	if (!ft_strcmp(cmd->name, "env"))
		return (builtin_env(shell->env));
	else if (!ft_strcmp(cmd->name, "echo"))
		return (builtin_echo(cmd->args, shell));
	else if (!ft_strcmp(cmd->name, "pwd"))
		return (builtin_pwd());
	else if (!ft_strcmp(cmd->name, "cd"))
		return (builtin_cd(cmd->args, shell->env));
	else if (!ft_strcmp(cmd->name, "unset"))
		return (builtin_unset(&shell->env, cmd->args));
	else if (!ft_strcmp(cmd->name, "export"))
		return (builtin_export(&shell->env, cmd->args));
	else if (!ft_strcmp(cmd->name, "exit"))
		return (builtin_exit(shell, cmd->args, -1, -1));
	return (EXIT_FAILURE);
}

int	execute_parent_builtins(t_cmd *cmd, t_shell *shell, \
							int saved_in, int saved_out)
{
	if (!ft_strcmp(cmd->name, "cd"))
		return (builtin_cd(cmd->args, shell->env));
	else if (!ft_strcmp(cmd->name, "unset"))
		return (builtin_unset(&shell->env, cmd->args));
	else if (!ft_strcmp(cmd->name, "export"))
		return (builtin_export(&shell->env, cmd->args));
	else if (!ft_strcmp(cmd->name, "exit"))
		return (builtin_exit(shell, cmd->args, saved_in, saved_out));
	return (EXIT_FAILURE);
}
