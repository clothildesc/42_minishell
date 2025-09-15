/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cscache <cscache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 13:48:19 by cscache           #+#    #+#             */
/*   Updated: 2025/09/15 12:37:59 by cscache          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libft/libft.h"
#include "../../includes/minishell.h"

static void	handle_operators(t_lexer *lexer, char c)
{
	add_char(&lexer->tmp_token, c);
	if ((c == '<' && lexer->input[lexer->pos + 1] == '<') || \
		(c == '>' && lexer->input[lexer->pos + 1] == '>'))
	{
		(lexer->pos)++;
		add_char(&lexer->tmp_token, lexer->input[lexer->pos]);
	}
	create_token(lexer, true);
}

int	is_pos_char_operator(t_lexer *lexer, int position)
{
	char	next;

	next = lexer->input[position];
	return (next == '|' || next == '<' || next == '>');
}

static int	has_closing_quote(char *input, int start, char quote)
{
	int	i;

	i = start + 1;
	while (input[i])
	{
		if (input[i] == quote)
			return (1);
		i++;
	}
	return (0);
}

static void	enter_quote_state(t_lexer *lexer, char quote_char)
{
	if (lexer->state == NORMAL && lexer->tmp_token)
		create_token(lexer, true);
	if (!has_closing_quote(lexer->input, lexer->pos, quote_char))
	{
		write_error_missing_quote(lexer, quote_char);
		return ;
	}
	if (quote_char == '\'')
	{
		lexer->state = SINGLE_QUOTE;
		lexer->to_exp = false;
	}
	else
		lexer->state = DOUBLE_QUOTE;
}

void	process_normal_state(t_lexer *lexer)
{
	char	c;

	c = lexer->input[lexer->pos];
	if (c == '\'' || c == '"')
		enter_quote_state(lexer, c);
	else if (ft_isspace(c))
		create_token(lexer, false);
	else if (c == '|' || c == '<' || c == '>')
		handle_operators(lexer, c);
	else
	{
		add_char(&lexer->tmp_token, c);
		if (is_pos_char_operator(lexer, lexer->pos + 1))
			create_token(lexer, false);
	}
}
