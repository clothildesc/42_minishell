/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_tokens.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cscache <cscache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 13:48:19 by cscache           #+#    #+#             */
/*   Updated: 2025/09/15 17:11:42 by cscache          ###   ########.fr       */
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

static int	should_skip_empty_token(t_lexer *lexer)
{
	t_token	*last_token;
	int		consecutives_quotes;
	int		pos;

	last_token = NULL;
	if (lexer->tokens)
	{
		last_token = lexer->tokens;
		while (last_token->next)
			last_token = last_token->next;
	}
	if (!last_token || last_token->value[0] != '\0' || lexer->to_join != 0)
		return (0);
	consecutives_quotes = 0;
	pos = lexer->pos - 1;
	printf("[DEBUG] Start pos = %d\n", pos);
	while (pos >= 0 && (lexer->input[pos] == '"' || lexer->input[pos] == '\''))
	{
		consecutives_quotes++;
		pos--;
		printf("[DEBUG] Start pos = %d\n", pos);
	}
	printf("[DEBUG] End pos = %d - char %c\n", pos, lexer->input[pos]);
	return (consecutives_quotes > 2);
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
	{
		if (should_skip_empty_token(lexer))
			return ;
		create_empty_token(lexer);
	}
	ft_lstclear(&lexer->tmp_token, free);
	lexer->tmp_token = NULL;
	lexer->state = NORMAL;
	lexer->to_exp = true;
	lexer->to_join = false;
}
