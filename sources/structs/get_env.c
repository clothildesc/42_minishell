/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_env.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barmarti <barmarti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 20:42:11 by cscache           #+#    #+#             */
/*   Updated: 2025/09/12 10:50:39 by barmarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libft/libft.h"
#include "../../includes/minishell.h"

void	ft_lstadd_back_env(t_env **lst, t_env *new)
{
	t_env	*last;

	last = NULL;
	if (lst && new)
	{
		if (*lst)
		{
			last = *lst;
			while (last->next)
				last = last->next;
			last->next = new;
			new->prev = last;
		}
		else
			*lst = new;
	}
}

static void	free_new(t_env *new)
{
	if (new->key)
		free(new->key);
	if (new->value)
		free(new->value);
	free(new);
}

static void	create_env_node(char *env, t_env **head)
{
	int		i;
	t_env	*new;

	new = malloc(sizeof(t_env));
	if (!new)
		return ;
	ft_bzero(new, sizeof(t_env));
	i = 0;
	while (env[i])
	{
		if (env[i] == '=')
		{
			new->key = ft_strndup(env, i);
			new->value = ft_strdup(&env[i + 1]);
			if (!new->key || !new->value)
			{
				free_new(new);
				return ;
			}
			break ;
		}
		i++;
	}
	ft_lstadd_back_env(head, new);
}

t_env	*get_env(char **envp)
{
	int		i;
	t_env	*env;

	env = NULL;
	if (!envp)
		return (NULL);
	i = 0;
	while (envp[i])
	{
		create_env_node(envp[i], &env);
		i++;
	}
	return (env);
}
