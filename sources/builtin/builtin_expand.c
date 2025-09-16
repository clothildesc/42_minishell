/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_expand.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cscache <cscache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 16:50:06 by barmarti          #+#    #+#             */
/*   Updated: 2025/09/16 14:07:30 by cscache          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libft/libft.h"
#include "../../includes/minishell.h"

static char	*get_var_to_expand(char *input, int var_end_index)
{
	char	*var_to_exp;

	if (!input)
		return (NULL);
	var_to_exp = ft_substr(input, 0, var_end_index);
	if (!var_to_exp)
		return (NULL);
	return (var_to_exp);
}

static char	*find_env_var(t_env *env, char *var_to_exp, \
						char *input, int var_end_index)
{
	char	*result;

	while (env)
	{
		if (!ft_strcmp(var_to_exp, env->key))
		{
			result = ft_strjoin(env->value, &input[var_end_index]);
			if (!result)
				return (NULL);
			return (result);
		}
		env = env->next;
	}
	result = ft_strjoin("", &input[var_end_index]);
	if (!result)
		return (NULL);
	return (result);
}

int	get_var_end_index(char *input)
{
	int	i;

	i = 0;
	if (input[i] && input[i] == '?')
		return (1);
	while (input[i] && (ft_isalnum(input[i]) || input[i] == '_'))
		i++;
	return (i);
}

static char	*get_expanded_result(t_shell *shell, char *input)
{
	char	*var_to_exp;
	char	*result;
	char	*status_str;
	int		var_end_index;

	if (!shell->env || !shell || !input)
		return (NULL);
	var_end_index = get_var_end_index(input);
	if (var_end_index == 0)
		return (NULL);
	var_to_exp = get_var_to_expand(input, var_end_index);
	if (!var_to_exp)
		return (NULL);
	if (!ft_strcmp(var_to_exp, "?"))
	{
		status_str = ft_itoa(shell->prev_status);
		result = ft_strjoin(status_str, &input[var_end_index]);
		free(status_str);
	}
	else
		result = find_env_var(shell->env, var_to_exp, input, var_end_index);
	free(var_to_exp);
	return (result);
}

char	*builtin_expand(char *input, t_shell *shell, char *result)
{
	char	*exp;
	int		dollar_index;
	char	*new_result;

	if (!shell->env || !input)
		return (result);
	dollar_index = get_char_index(input, '$');
	if (dollar_index < 0)
		return (result);
	exp = get_expanded_result(shell, &input[dollar_index + 1]);
	if (!exp)
		return (result);
	if (!result)
	{
		result = ft_strdup(input);
		if (!result)
			return (free(exp), NULL);
	}
	new_result = ft_strnjoin(result, exp, dollar_index);
	free(result);
	free(exp);
	if (!new_result)
		return (NULL);
	return (builtin_expand(new_result, shell, new_result));
}
