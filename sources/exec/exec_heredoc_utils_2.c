/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_heredoc_utils_2.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cscache <cscache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/01 17:58:26 by cscache           #+#    #+#             */
/*   Updated: 2025/09/15 14:30:51 by cscache          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libft/libft.h"
#include "../../includes/minishell.h"

static bool	to_expand(char *line)
{
	int		dollar_index;

	dollar_index = get_char_index(line, '$');
	if (dollar_index < 0)
		return (false);
	return (true);
}

static int	process_heredoc_line(char **line, char *limiter, \
	t_shell *shell, int fd)
{
	char	*tmp;

	if (ft_strncmp(*line, limiter, ft_strlen(limiter)) == 0 \
		&& (*line)[ft_strlen(limiter)] == '\n')
	{
		free(*line);
		return (1);
	}
	if (to_expand(*line))
	{
		tmp = builtin_expand(*line, shell, NULL);
		free(*line);
		*line = ft_strdup(tmp);
		if (!*line)
		{
			free(tmp);
			return (-1);
		}
		free(tmp);
	}
	write(fd, *line, ft_strlen(*line));
	free(*line);
	return (0);
}

static void	read_and_write_heredoc(int fd, char *limiter, t_shell *shell)
{
	char	*line;
	int		limiter_reached;
	int		result;

	limiter_reached = 0;
	remove_echoctl();
	while (true)
	{
		ft_putstr_fd("> ", STDOUT_FILENO);
		line = get_next_line(STDIN_FILENO);
		if (!line)
			break ;
		result = process_heredoc_line(&line, limiter, shell, fd);
		if (result == 1)
		{
			limiter_reached = 1;
			break ;
		}
		else if (result == -1)
			return ;
	}
	active_echoctl();
	if (!limiter_reached && g_signal_received != 130)
		ft_putendl_fd("\nminishell: warning: here-doc delimited by eof", 2);
}

void	close_prev_fd_heredoc(t_ast *node)
{
	t_cmd	*cmd;

	if (!node)
		return ;
	if (node->node_type == NODE_PIPE)
	{
		close_prev_fd_heredoc(node->data.binary.left);
		close_prev_fd_heredoc(node->data.binary.right);
	}
	else if (node->node_type == NODE_CMD)
	{
		cmd = node->data.cmd.cmd;
		ft_close_fd(&cmd->fd_heredoc);
	}
}

pid_t	execute_child_heredoc(t_shell *shell, char *limiter, int fd_heredoc, \
	char *file)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
	{
		perror("fork heredoc");
		return (-1);
	}
	if (pid == 0)
	{
		set_up_signals_child(true);
		close_prev_fd_heredoc(shell->ast);
		read_and_write_heredoc(fd_heredoc, limiter, shell);
		ft_close_fd(&fd_heredoc);
		close_all_command_fds(shell->ast);
		free(file);
		if (g_signal_received)
			free_and_exit(shell, g_signal_received);
		else
			free_and_exit(shell, EXIT_SUCCESS);
	}
	return (pid);
}
