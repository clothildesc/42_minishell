/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_cmd_utils_2.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cscache <cscache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 17:01:17 by barmarti          #+#    #+#             */
/*   Updated: 2025/09/15 12:57:42 by cscache          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libft/libft.h"
#include "../../includes/minishell.h"

static t_arg	*init_new_arg(void)
{
	t_arg	*new_arg;

	new_arg = malloc(sizeof(t_arg));
	if (!new_arg)
		return (NULL);
	ft_bzero(new_arg, sizeof(t_arg));
	return (new_arg);
}

void	create_args_lst(t_arg **args, t_token *token, t_shell *shell)
{
	t_arg	*new_arg;

	new_arg = init_new_arg();
	if (!new_arg)
		return ;
	if (token->to_exp == true)
		get_exp_value(token, shell, new_arg);
	else
		new_arg->arg = ft_strdup(token->value);
	if (!new_arg->arg)
	{
		free(new_arg);
		clear_args_lst(args);
		return ;
	}
	if (token->to_exp == true && token->to_join == false \
		&& new_arg->arg[0] == '\0')
	{
		free(new_arg->arg);
		free(new_arg);
		return ;
	}
	new_arg->to_join = token->to_join;
	ft_lstadd_args(args, new_arg);
}

static int	args_lst_size(t_arg *args)
{
	int	size;

	size = 0;
	while (args)
	{
		size++;
		args = args->next;
	}
	return (size);
}

static char	**fill_args_array(char **args_array, t_arg *args)
{
	int		i;
	t_arg	*current;

	i = 0;
	current = args;
	while (current)
	{
		args_array[i] = ft_strdup(current->arg);
		if (!args_array[i])
		{
			free_tab_chars(args_array);
			return (NULL);
		}
		i++;
		current = current->next;
	}
	args_array[i] = NULL;
	return (args_array);
}

void	lst_args_to_array(t_cmd *cmd, t_arg **args)
{
	char	**args_array;

	args_array = (char **)malloc(sizeof(char *) * (args_lst_size(*args) + 1));
	if (!args_array)
	{
		clear_args_lst(args);
		return ;
	}
	cmd->args = fill_args_array(args_array, *args);
	if (!cmd->args)
	{
		clear_args_lst(args);
		free(args_array);
		return ;
	}
	clear_args_lst(args);
}
