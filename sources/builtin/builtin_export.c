/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cscache <cscache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 19:09:25 by barmarti          #+#    #+#             */
/*   Updated: 2025/09/16 15:41:48 by cscache          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libft/libft.h"
#include "../../includes/minishell.h"

static t_env	*find_or_create_env(t_env **env, char *input, char *key)
{
	t_env	*tmp;

	if (!input || !key)
		return (NULL);
	tmp = *env;
	while (tmp)
	{
		if (!ft_strcmp(key, tmp->key))
		{
			update_env_value(input, &tmp);
			free(key);
			return (NULL);
		}
		tmp = tmp->next;
	}
	tmp = malloc(sizeof(t_env));
	if (!tmp)
		return (NULL);
	ft_bzero(tmp, sizeof(t_env));
	return (create_new_env_node(tmp, input, key));
}

static int	not_valid_identifier(char *input)
{
	ft_putstr_fd("minishell: export: '", 2);
	ft_putstr_fd(input, 2);
	ft_putendl_fd("': not a valid identifier", 2);
	return (0);
}

static int	key_name_is_valid(char *key, char *input)
{
	int	i;

	i = 0;
	if (key[0] == '\0')
		return (not_valid_identifier(input));
	while (key[i])
	{
		if (!ft_isalpha(key[0]) && key[0] != '_')
			return (not_valid_identifier(input));
		else if (!ft_isalpha(key[i]) && !ft_isdigit(key[i]) && key[i] != '_')
			return (not_valid_identifier(input));
		else
			i++;
	}
	return (1);
}

static void	handle_export_key(t_env **env, char *arg, char *key, \
								int *exit_code)
{
	t_env	*new;

	if (!key_name_is_valid(key, arg))
	{
		*exit_code = EXIT_FAILURE;
		free(key);
	}
	else
	{
		new = find_or_create_env(env, arg, key);
		if (new)
			ft_lstadd_back_env(env, new);
	}
}

int	builtin_export(t_env **env, char **args)
{
	char	*key;
	int		i;
	int		exit_code;

	exit_code = EXIT_SUCCESS;
	if (!args)
		return (exit_code);
	if (args && !args[1])
		return (print_env_export(*env));
	i = 1;
	while (args[i])
	{
		key = get_input_key(args[i]);
		if (!key)
			return (EXIT_SUCCESS);
		handle_export_key(env, args[i], key, &exit_code);
		i++;
	}
	return (exit_code);
}
