/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_cmd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cscache <cscache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/15 20:03:10 by cscache           #+#    #+#             */
/*   Updated: 2025/09/15 14:37:12 by cscache          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libft/libft.h"
#include "../../includes/minishell.h"

static t_ast	*init_cmd_node(void)
{
	t_ast	*new_cmd;

	new_cmd = malloc(sizeof(t_ast));
	if (!new_cmd)
		return (NULL);
	ft_bzero(new_cmd, sizeof(t_ast));
	new_cmd->node_type = NODE_CMD;
	new_cmd->data.cmd.cmd = malloc(sizeof(t_cmd));
	if (!new_cmd->data.cmd.cmd)
	{
		free(new_cmd);
		return (NULL);
	}
	ft_bzero(new_cmd->data.cmd.cmd, sizeof(t_cmd));
	new_cmd->data.cmd.cmd->fd_in = -1;
	new_cmd->data.cmd.cmd->fd_out = -1;
	new_cmd->data.cmd.cmd->fd_heredoc = -1;
	return (new_cmd);
}

static t_token	*process_token(t_token *current, t_ast *new_cmd, t_arg **args, \
	t_shell *shell)
{
	if (current->type == WORD)
	{
		create_args_lst(args, current, shell);
		return (current->next);
	}
	else if (current->type == REDIR_IN || current->type == REDIR_OUT || \
			current->type == HERE_DOC || current->type == APPEND_OUT)
	{
		create_redir_lst(current, new_cmd->data.cmd.cmd);
		current = current->next;
		if (current)
			current = current->next;
		return (current);
	}
	return (NULL);
}

static void	finalize_cmd_args(t_cmd *cmd, t_arg **args)
{
	if (*args)
	{
		if (!cmd->name)
			set_cmd_name(cmd, *args);
		lst_args_to_array(cmd, args);
	}
}

t_ast	*parse_cmd(t_token **tokens, t_shell *shell)
{
	t_token	*current;
	t_ast	*new_cmd;
	t_arg	*args;

	args = NULL;
	shell->nb_cmds++;
	current = *tokens;
	new_cmd = init_cmd_node();
	if (!new_cmd)
		return (NULL);
	while (current && current->type != PIPE)
	{
		current = process_token(current, new_cmd, &args, shell);
		if (!current)
			break ;
	}
	finalize_cmd_args(new_cmd->data.cmd.cmd, &args);
	*tokens = current;
	return (new_cmd);
}
