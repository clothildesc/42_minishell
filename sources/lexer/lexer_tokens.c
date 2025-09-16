/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_tokens.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cscache <cscache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 13:48:19 by cscache           #+#    #+#             */
/*   Updated: 2025/09/16 14:24:14 by cscache          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libft/libft.h"
#include "../../includes/minishell.h"

static void	add_to_lst_tokens(t_token **lst, t_token *new)
{
	t_token	*last;

	last = NULL;
	if (*lst)
	{
		last = *lst;
		while (last->next)
			last = last->next;
		new->prev = last;
		last->next = new;
	}
	else
		*lst = new;
}

static void	create_new_token(t_lexer *lexer)
{
	char	*token_value;
	t_token	*new_token;

	token_value = create_token_value(lexer);
	if (!token_value)
	{
		ft_lstclear(&lexer->tmp_token, free);
		return ;
	}
	new_token = malloc(sizeof(t_token));
	if (!new_token)
	{
		ft_lstclear(&lexer->tmp_token, free);
		free(token_value);
		return ;
	}
	ft_bzero(new_token, sizeof(t_token));
	new_token->value = token_value;
	new_token->to_exp = lexer->to_exp;
	new_token->to_join = lexer->to_join;
	new_token->type = determine_token_type(lexer);
	add_to_lst_tokens(&lexer->tokens, new_token);
}

static void	create_empty_token(t_lexer *lexer)
{
	t_token	*new_token;

	new_token = malloc(sizeof(t_token));
	if (!new_token)
		return ;
	ft_bzero(new_token, sizeof(t_token));
	new_token->value = ft_strdup("");
	if (!new_token->value)
	{
		free(new_token);
		return ;
	}
	new_token->to_exp = false;
	new_token->to_join = lexer->to_join;
	new_token->type = WORD;
	add_to_lst_tokens(&lexer->tokens, new_token);
}

void	create_token(t_lexer *lexer, bool to_join)
{
	if (to_join)
		set_to_join(lexer);
	else
		lexer->to_join = false;
	if (lexer->tmp_token)
		create_new_token(lexer);
	else if (to_join && lexer->state == NORMAL)
		create_empty_token(lexer);
	ft_lstclear(&lexer->tmp_token, free);
	lexer->tmp_token = NULL;
	lexer->state = NORMAL;
	lexer->to_exp = true;
	lexer->to_join = false;
}
