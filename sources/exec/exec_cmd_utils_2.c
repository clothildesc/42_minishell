/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd_utils_2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cscache <cscache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 09:54:18 by cscache           #+#    #+#             */
/*   Updated: 2025/09/17 09:54:40 by cscache          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	cmd_not_found(t_cmd *cmd)
{
	int		dollar_index;
	char	*cmd_cut;

	ft_putstr_fd("minishell: ", 2);
	if (cmd->name)
	{
		dollar_index = get_char_index(cmd->name, '$');
		if (dollar_index > 0)
		{
			cmd_cut = ft_substr(cmd->name, 0, dollar_index);
			if (cmd_cut)
			{
				ft_putstr_fd(cmd_cut, 2);
				free(cmd_cut);
			}
		}
		else
			ft_putstr_fd(cmd->name, 2);
	}
	ft_putendl_fd(": command not found", 2);
	clear_cmd(cmd);
	return (EXIT_CMD_NOT_FOUND);
}

int	get_exit_code(int status)
{
	int	sig;

	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	else if (WIFSIGNALED(status))
	{
		sig = WTERMSIG(status);
		if (sig == SIGQUIT)
			write(STDERR_FILENO, "Quit (core dumped)\n", 20);
		else if (sig == SIGINT)
			write(STDOUT_FILENO, "\n", 1);
		g_signal_received = 128 + sig;
		return (g_signal_received);
	}
	else
		return (EXIT_FAILURE);
}
