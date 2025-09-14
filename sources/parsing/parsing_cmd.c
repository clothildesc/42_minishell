/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_cmd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cscache <cscache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/15 20:03:10 by cscache           #+#    #+#             */
/*   Updated: 2025/09/14 20:16:17 by cscache          ###   ########.fr       */
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

static int	is_redir(t_token_type type)
{
	return (type == REDIR_IN || type == REDIR_OUT || \
			type == HERE_DOC || type == APPEND_OUT);
}

static void	process_word(t_arg **args, t_token **current, \
						t_shell *shell)
{
	create_args_lst(args, *current, shell);
	*current = (*current)->next;
}

static void	process_redir(t_token **current, t_cmd *new_cmd)
{
	create_redir_lst(*current, new_cmd);
	*current = (*current)->next->next;
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
		if (current->type == WORD)
			process_word(&args, &current, shell);
		else if (is_redir(current->type))
			process_redir(&current, new_cmd->data.cmd.cmd);
		else
			break ;
	}
	if (args)
	{
		if (!new_cmd->data.cmd.cmd->name)
			set_cmd_name(new_cmd->data.cmd.cmd, args);
		lst_args_to_array(new_cmd->data.cmd.cmd, &args);
	}
	*tokens = current;
	return (new_cmd);
}
