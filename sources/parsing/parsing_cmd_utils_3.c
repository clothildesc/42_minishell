/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_cmd_utils_3.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cscache <cscache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 17:05:25 by barmarti          #+#    #+#             */
/*   Updated: 2025/09/12 19:08:24 by cscache          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libft/libft.h"
#include "../../includes/minishell.h"

void	get_exp_value(t_token *token, t_shell *shell, t_arg *new_arg)
{
	char	*src;
	char	*input;

	input = ft_strdup(token->value);
	if (!input)
		return ;
	src = builtin_expand(input, shell, NULL);
	if (input)
		free(input);
	if (src)
	{
		new_arg->arg = ft_strdup(src);
		free(src);
		return ;
	}
	new_arg->arg = ft_strdup(token->value);
}
