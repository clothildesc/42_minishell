/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_tokens_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cscache <cscache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 10:58:27 by cscache           #+#    #+#             */
/*   Updated: 2025/09/15 12:38:16 by cscache          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libft/libft.h"
#include "../../includes/minishell.h"

void	add_char(t_list **tmp_token, char c)
{
	t_list	*new;
	char	*new_char;

	new_char = malloc(sizeof(char));
	if (!new_char)
		return ;
	*new_char = c;
	new = ft_lstnew(new_char);
	if (!new)
	{
		free(new_char);
		new_char = NULL;
		return ;
	}
	ft_lstadd_back(tmp_token, new);
}

t_token_type	determine_token_type(t_lexer *lexer)
{
	char	first;

	if (!lexer->tmp_token)
		return (0);
	first = *(char *)lexer->tmp_token->content;
	if (first == '|')
		return (PIPE);
	if (first == '<')
	{
		if (lexer->tmp_token->next && \
			*(char *)lexer->tmp_token->next->content == '<')
			return (HERE_DOC);
		return (REDIR_IN);
	}
	if (first == '>')
	{
		if (lexer->tmp_token->next \
			&& *(char *)lexer->tmp_token->next->content == '>')
			return (APPEND_OUT);
		return (REDIR_OUT);
	}
	return (WORD);
}

void	clear_tokens_lst(t_token **tokens)
{
	t_token	*next;
	t_token	*current;

	if (!tokens || !*tokens)
		return ;
	current = *tokens;
	while (current)
	{
		next = current->next;
		if (current->value)
			free(current->value);
		free(current);
		current = next;
	}
	*tokens = NULL;
}

char	*create_token_value(t_lexer *lexer)
{
	char		*token_value;
	t_list		*current;
	int			i;

	if (!(lexer->tmp_token))
		return (NULL);
	token_value = malloc(sizeof(char) * (ft_lstsize(lexer->tmp_token) + 1));
	if (!token_value)
		return (NULL);
	i = 0;
	current = lexer->tmp_token;
	while (current)
	{
		token_value[i++] = *(char *)current->content;
		current = current->next;
	}
	token_value[i] = 0;
	return (token_value);
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
