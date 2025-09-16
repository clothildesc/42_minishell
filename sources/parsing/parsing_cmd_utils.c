/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_cmd_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cscache <cscache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 17:01:17 by barmarti          #+#    #+#             */
/*   Updated: 2025/09/16 15:44:08 by cscache          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libft/libft.h"
#include "../../includes/minishell.h"

t_cmd	*set_cmd_name(t_cmd *new, t_arg *arg)
{
	if (!arg)
	{
		free(new);
		return (NULL);
	}
	new->name = ft_strdup(arg->arg);
	if (!new->name)
	{
		free(new);
		return (NULL);
	}
	return (new);
}

static void	ft_lstadd_redirs(t_redir **lst, t_redir *new)
{
	t_redir	*last;

	last = NULL;
	if (lst && new)
	{
		if (*lst)
		{
			last = *lst;
			while (last->next)
				last = last->next;
			last->next = new;
		}
		else
			*lst = new;
	}
}

void	create_redir_lst(t_token *token, t_cmd *cmd)
{
	t_redir	*new_redir;

	if (!token || !token->next || !cmd)
		return ;
	new_redir = malloc(sizeof(t_redir));
	if (!new_redir)
		return ;
	ft_bzero(new_redir, sizeof(t_redir));
	new_redir->target = ft_strdup(token->next->value);
	if (!new_redir->target)
	{
		free(new_redir);
		return ;
	}
	new_redir->type = token->type;
	new_redir->next = NULL;
	ft_lstadd_redirs(&cmd->redirs, new_redir);
}

static void	handle_to_join(t_arg *last, t_arg *new)
{
	char	*old_arg;

	old_arg = last->arg;
	last->arg = ft_strjoin(last->arg, new->arg);
	if (!last->arg)
	{
		last->arg = old_arg;
		free(new->arg);
		free(new);
		return ;
	}
	last->to_join = new->to_join;
	free(old_arg);
	free(new->arg);
	free(new);
}

void	ft_lstadd_args(t_arg **args, t_arg *new)
{
	t_arg	*last;

	last = NULL;
	if (args && new)
	{
		if (*args)
		{
			last = *args;
			while (last->next)
				last = last->next;
			if (last->to_join)
				handle_to_join(last, new);
			else
				last->next = new;
		}
		else
			*args = new;
	}
}
