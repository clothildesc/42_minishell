/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cscache <cscache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 11:41:18 by cscache           #+#    #+#             */
/*   Updated: 2025/09/17 09:54:04 by cscache          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

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
	if (prepare_redirections(cmd) == -1)
		return (EXIT_FAILURE);
	status = prepare_cmd(cmd, shell->env);
	if (status != EXIT_SUCCESS)
	{
		close_files(cmd);
		return (status);
	}
	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		close_files(cmd);
		return (EXIT_FAILURE);
	}
	if (pid == 0)
		execute_child(cmd, shell, fd_i, fd_o);
	shell->pids[shell->pid_index++] = pid;
	return (EXIT_SUCCESS);
}

static int	handle_empty_cmd(t_cmd *cmd)
{
	if (cmd->redirs)
	{
		if (prepare_redirections(cmd) == -1)
			return (EXIT_FAILURE);
		close_files(cmd);
		return (EXIT_SUCCESS);
	}
	cmd_not_found(cmd);
	return (EXIT_CMD_NOT_FOUND);
}

static int	handle_dollar_cmd(t_cmd *cmd)
{
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
	return (0);
}

int	execute_cmd(t_ast *node, t_shell *shell, int fd_i, int fd_o)
{
	t_cmd	*cmd;
	int		result;

	if (!node)
		return (EXIT_FAILURE);
	cmd = node->data.cmd.cmd;
	if (!cmd->name || cmd->name[0] == '\0')
		return (handle_empty_cmd(cmd));
	result = handle_dollar_cmd(cmd);
	if (result != 0)
		return (result);
	if (is_a_builtin(cmd->name))
		return (exec_builtin_simple(cmd, shell, fd_i, fd_o));
	return (fork_and_execute(cmd, shell, fd_i, fd_o));
}
