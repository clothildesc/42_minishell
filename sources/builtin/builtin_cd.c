/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barmarti <barmarti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 16:38:45 by cscache           #+#    #+#             */
/*   Updated: 2025/09/12 10:26:53 by barmarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libft/libft.h"
#include "../../includes/minishell.h"

void	update_vars_pwd(t_env **head, char *key)
{
	t_env	*to_update;
	t_env	*new;
	char	*cwd;

	if (!head || !*head)
		return ;
	to_update = get_node(head, key);
	free(to_update->value);
	if (!ft_strcmp(key, "OLDPWD"))
	{
		new = get_node(head, "PWD");
		to_update->value = ft_strdup(new->value);
	}
	else
	{
		cwd = getcwd(NULL, 0);
		if (cwd)
		{
			to_update->value = ft_strdup(cwd);
			free(cwd);
		}
	}
}

char	*get_env_value(t_env *env, char *key)
{
	while (env)
	{
		if (!ft_strcmp(env->key, key))
			return (env->value);
		env = env->next;
	}
	return (NULL);
}

int	builtin_cd(char **args, t_env *env)
{
	char	*path;
	int		i;

	update_vars_pwd(&env, "OLDPWD");
	if (args && !args[1])
		path = get_env_value(env, "HOME");
	else
	{
		i = 1;
		while (args[i])
			i++;
		if (i > 2)
		{
			ft_putendl_fd(ERROR_CD_MANY_ARGS, 2);
			return (EXIT_FAILURE);
		}
		path = args[1];
	}
	if (chdir(path) == -1)
	{
		perror("cd");
		return (EXIT_FAILURE);
	}
	update_vars_pwd(&env, "PWD");
	return (EXIT_SUCCESS);
}
