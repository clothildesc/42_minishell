/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_tokens.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cscache <cscache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 13:48:19 by cscache           #+#    #+#             */
/*   Updated: 2025/09/14 19:29:37 by cscache          ###   ########.fr       */
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

static void	reset_tmp_token(t_lexer *lexer)
{
	ft_lstclear(&lexer->tmp_token, free);
	lexer->tmp_token = NULL;
	lexer->state = NORMAL;
	lexer->to_exp = true;
	lexer->to_join = false;
}

void	set_to_join(t_lexer *lexer)
{
	int		pos;

	pos = lexer->pos;
	while (lexer->input[pos] && (lexer->input[pos] == '"' || \
		lexer->input[pos] == '\''))
		pos++;
	if (lexer->input[pos] && !ft_isspace(lexer->input[pos]))
		lexer->to_join = true;
	else
		lexer->to_join = false;
}

static t_token	*set_new_token(t_lexer *lexer, t_token *new_token, \
	char *token_value)
{
	ft_bzero(new_token, sizeof(t_token));
	new_token->value = token_value;
	new_token->to_exp = lexer->to_exp;
	new_token->to_join = lexer->to_join;
	new_token->type = determine_token_type(lexer);
	add_to_lst_tokens(&lexer->tokens, new_token);
	return (new_token);
}

void	create_token(t_lexer *lexer, bool to_join)
{
	char	*token_value;
	t_token	*new_token;
	t_token	*last_token = NULL;

	if (to_join)
		set_to_join(lexer);
	else
		lexer->to_join = false;
	if (lexer->tokens)
	{
		last_token = lexer->tokens;
		while (last_token->next)
			last_token = last_token->next;
	}
	if (lexer->tmp_token)
	{
		token_value = create_token_value(lexer);
		printf("[DEBUG] create_token tmp_token (%s)\nlexer_to_join = %d\n", token_value, lexer->to_join);
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
		set_new_token(lexer, new_token, token_value);
	}
	else if (to_join && lexer->state == NORMAL)
	{
		printf("[DEBUG] create_token no tmp_token (0 - NULL)\nlexer_to_join = %d\n", lexer->to_join);
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
		if (last_token && last_token->value[0] == '\0')
			new_token->to_join = false;
		else
			new_token->to_join = lexer->to_join;
		new_token->type = WORD;
		add_to_lst_tokens(&lexer->tokens, new_token);
	}
	reset_tmp_token(lexer);
}
