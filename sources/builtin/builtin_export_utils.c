/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cscache <cscache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 15:39:16 by barmarti          #+#    #+#             */
/*   Updated: 2025/09/16 17:44:19 by cscache          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libft/libft.h"
#include "../../includes/minishell.h"

t_env	*get_node(t_env **head, char *key)
{
	t_env	*current;

	if (!head || !*head || !key)
		return (NULL);
	current = *head;
	while (current)
	{
		if (!ft_strcmp(key, current->key))
			return (current);
		current = current->next;
	}
	return (NULL);
}

int	value_to_append(char *input)
{
	int	i;

	i = 1;
	if (!input)
		return (0);
	while (input[i])
	{
		if (input[i] == '=' && input[i - 1] == '+')
			return (1);
		i++;
	}
	return (0);
}

char	*get_input_value(char *input)
{
	int		i;
	char	*value;

	i = 0;
	if (!input)
		return (NULL);
	while (input[i])
	{
		if (input[i] == '=')
		{
			value = ft_strdup(&input[i + 1]);
			if (!value)
				return (NULL);
			return (value);
		}
		i++;
	}
	value = ft_strdup("");
	if (!value)
		return (NULL);
	return (value);
}

char	*get_input_key(char *input)
{
	int	i;

	i = 0;
	if (!input)
		return (NULL);
	while (input[i] && input[i] != '=')
		i++;
	if (value_to_append(input))
		return (ft_strndup(input, i - 1));
	else
		return (ft_strndup(input, i));
}

void	update_env_value(char *input, t_env **node)
{
	char	*new_value;
	char	*old_value;

	if (!input || !node || !*node)
		return ;
	new_value = get_input_value(input);
	if (!new_value)
		return ;
	if (value_to_append(input))
	{
		if (!new_value)
			return ;
		old_value = (*node)->value;
		(*node)->value = ft_strjoin((*node)->value, new_value);
		free(old_value);
		free(new_value);
	}
	else
	{
		free((*node)->value);
		(*node)->value = new_value;
	}
	if (!(*node)->value)
		return ;
}
