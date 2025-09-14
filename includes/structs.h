/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structs.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barmarti <barmarti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 10:01:36 by barmarti          #+#    #+#             */
/*   Updated: 2025/09/14 17:32:20 by barmarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCTS_H
# define STRUCTS_H

# include "minishell.h"

/*=============== LEXER =============== */

typedef enum e_state
{
	NORMAL,
	SINGLE_QUOTE,
	DOUBLE_QUOTE
}	t_state;

typedef enum e_token_type
{
	WORD,
	PIPE,
	HERE_DOC,
	APPEND_OUT,
	REDIR_IN,
	REDIR_OUT,
}	t_token_type;

typedef struct s_token
{
	char			*value;
	t_token_type	type;
	bool			to_exp;
	bool			to_join;
	struct s_token	*prev;
	struct s_token	*next;
}	t_token;

typedef struct s_lexer
{
	t_state			state;
	t_list			*tmp_token;
	t_token			*tokens;
	int				pos;
	bool			to_exp;
	bool			to_join;
	char			*input;
	int				error;
}	t_lexer;

/*=============== ENV =============== */

typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*prev;
	struct s_env	*next;
}	t_env;

/*=============== PARSING =============== */

typedef enum e_node_type
{
	NODE_CMD,
	NODE_PIPE
}	t_node_type;

typedef struct s_args
{
	char			*arg;
	bool			to_join;
	struct s_args	*next;
}	t_arg;

typedef struct s_redir
{
	t_token_type	type;
	char			*target;
	struct s_redir	*next;
}	t_redir;

typedef struct s_cmd
{
	char			*name;
	char			**args;
	t_redir			*redirs;
	char			*abs_path;
	int				fd_in;
	int				fd_out;
	int				fd_heredoc;
	pid_t			pid;
	pid_t			pid_heredoc;
	int				exit_status;
}	t_cmd;

struct	s_ast;

typedef struct s_ast_binary
{
	struct s_ast	*left;
	struct s_ast	*right;
}	t_ast_binary;

typedef struct s_ast_unary_cmd
{
	t_cmd	*cmd;
}	t_ast_unary_cmd;

typedef union u_ast_data
{
	t_ast_binary	binary;
	t_ast_unary_cmd	cmd;
}	t_ast_data;

typedef struct s_ast_unary
{
	t_cmd	*cmd;
}	t_ast_unary;

typedef struct s_ast
{
	t_node_type		node_type;
	t_ast_data		data;
}	t_ast;

/*=============== EXEC =============== */

typedef struct s_shell
{
	t_lexer	lexer;
	t_token	*tokens;
	t_ast	*ast;
	t_env	*env;
	int		nb_cmds;
	pid_t	*pids;
	int		pid_index;
	int		pipes[1024][2];
	int		nb_pipes;
	int		prev_status;
	int		status;
}	t_shell;

#endif