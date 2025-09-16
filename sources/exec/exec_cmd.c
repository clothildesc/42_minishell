/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cscache <cscache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 11:41:18 by cscache           #+#    #+#             */
/*   Updated: 2025/09/16 15:44:38 by cscache          ###   ########.fr       */
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

static void	execute_child(t_cmd *cmd, t_shell *shell, int fd_i, int fd_o)
{
	char	**env_array;

	set_up_signals_child(false);
	env_array = lst_env_to_array(shell->env);
	if (!env_array)
	{
		perror("malloc env_array");
		free_and_exit(shell, EXIT_FAILURE);
	}
	if (prepare_redirections(cmd) == -1)
		free_child_and_exit(shell, env_array, EXIT_FAILURE);
	manage_dup(cmd, fd_i, fd_o);
	close_all_fds_and_pipes(shell);
	execve(cmd->abs_path, cmd->args, env_array);
	perror("execve");
	free_child_and_exit(shell, env_array, EXIT_CMD_NOT_FOUND);
}

static int	fork_and_execute(t_cmd *cmd, t_shell *shell, int fd_i, int fd_o)
{
	pid_t	pid;
	int		status;

	if (!shell->env)
		return (cmd_not_found(cmd));
	status = prepare_cmd(cmd, shell->env);
	if (status != EXIT_SUCCESS)
		return (status);
	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		return (EXIT_FAILURE);
	}
	if (pid == 0)
		execute_child(cmd, shell, fd_i, fd_o);
	shell->pids[shell->pid_index++] = pid;
	return (EXIT_SUCCESS);
}

int	execute_cmd(t_ast *node, t_shell *shell, int fd_i, int fd_o)
{
	t_cmd	*cmd;

	if (!node)
		return (EXIT_FAILURE);
	cmd = node->data.cmd.cmd;
	if (!cmd->name || cmd->name[0] == '\0')
	{
		cmd_not_found(cmd);
		return (EXIT_CMD_NOT_FOUND);
	}
	if (cmd->name[0] == '$' && cmd->name[1])
	{
		clear_cmd(cmd);
		return (EXIT_SUCCESS);
	}
	if (ft_strchr(cmd->name, '$'))
	{
		cmd_not_found(cmd);
		return (EXIT_CMD_NOT_FOUND);
	}
	if (is_a_builtin(cmd->name))
		return (exec_builtin_simple(cmd, shell, fd_i, fd_o));
	return (fork_and_execute(cmd, shell, fd_i, fd_o));
}
