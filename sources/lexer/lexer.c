/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cscache <cscache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 13:48:19 by cscache           #+#    #+#             */
/*   Updated: 2025/09/15 11:34:49 by cscache          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libft/libft.h"
#include "../../includes/minishell.h"

static void	process_single_quote_state(t_lexer *lexer)
{
	char	c;

	c = lexer->input[lexer->pos];
	if (c == '\'')
	{
		lexer->state = NORMAL;
		create_token(lexer, true);
	}
	else
		add_char(&lexer->tmp_token, c);
}

static void	process_double_quote_state(t_lexer *lexer)
{
	char	c;

	c = lexer->input[lexer->pos];
	if (c == '"')
	{
		lexer->state = NORMAL;
		create_token(lexer, true);
	}
	else
		add_char(&lexer->tmp_token, c);
}

void	write_error_missing_quote(t_lexer *lexer, char quote_char)
{
	ft_putstr_fd("error: missing ", 2);
	if (quote_char == '\'')
		ft_putendl_fd("single quote", 2);
	else if (quote_char == '"')
		ft_putendl_fd("double quote", 2);
	if (lexer->tmp_token)
	{
		ft_lstclear(&(lexer->tmp_token), free);
		lexer->tmp_token = NULL;
	}
	if (lexer->tokens)
		clear_tokens_lst(&lexer->tokens);
	lexer->state = NORMAL;
	lexer->error = 1;
}

static void	process_current_char(t_lexer *lexer)
{
	if (lexer->state == NORMAL)
		process_normal_state(lexer);
	else if (lexer->state == SINGLE_QUOTE)
		process_single_quote_state(lexer);
	else if (lexer->state == DOUBLE_QUOTE)
		process_double_quote_state(lexer);
}

t_token	*ft_lexer(char *input, t_shell *shell)
{
	t_lexer		lexer;
	char		c;

	if (!shell || !input)
		return (NULL);
	ft_bzero(&lexer, sizeof(t_lexer));
	lexer.state = NORMAL;
	lexer.input = input;
	lexer.to_exp = true;
	lexer.to_join = false;
	lexer.pos = 0;
	c = lexer.input[lexer.pos];
	while (lexer.input[lexer.pos] && !lexer.error)
	{
		process_current_char(&lexer);
		(lexer.pos)++;
	}
	if (lexer.error)
	{
		shell->status = EXIT_FAILURE;
		return (NULL);
	}
	create_token(&lexer, false);
	return (lexer.tokens);
}
