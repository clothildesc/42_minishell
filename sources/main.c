/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cscache <cscache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 10:44:11 by cscache           #+#    #+#             */
/*   Updated: 2025/09/12 15:58:24 by cscache          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	g_signal_received = 0;

int	main(int ac, char **av, char **envp)
{
	t_shell	shell;
	int		status;

	(void)av;
	if (ac == 1)
	{
		init_shell(&shell, envp);
		status = main_loop(&shell);
		clear_shell(&shell);
		rl_clear_history();
		if (status == EXIT_FAILURE)
			return (EXIT_FAILURE);
	}
	return (shell.status);
}
