/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cscache <cscache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 17:00:27 by cscache           #+#    #+#             */
/*   Updated: 2025/09/15 12:56:24 by cscache          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <termios.h>
# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <stdbool.h>
# include <limits.h>
# include <signal.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <fcntl.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include "../libft/libft.h"
# include "structs.h"

/*=============== EXIT CODES =============== */
# define EXIT_SUCCESS 			0
# define EXIT_FAILURE 			1
# define EXIT_SYNTAX_ERROR		2
# define EXIT_CMD_NOT_FOUND		127
# define EXIT_PERMISSION_DENIED 126
# define EXIT_SIGNAL			128
# define EXIT_CTRL_C 			130
# define EXIT_CTRL_D 			131

# define EOF_RECEIVED			3

/*=============== ERRORS =============== */

# define SYNTAX_ERROR_TOKEN "minishell: syntax error near unexpected token "
# define SYNTAX_ERROR_KEY_ENV "minishell: export: not a valid identifier"
# define ERROR_CD_MANY_ARGS "minishell: cd: too many arguments"
# define ERROR_MISSING_FILE "minishell: No such file or directory"

/*=============== GLOBAL VARIABLE =============== */

extern int	g_signal_received;

/*=============== FUNCTIONS =============== */
/*-------Signal-------*/
void			set_up_signals_parent(void);
void			set_up_signals_child(bool heredoc);
void			ft_handler_sigint(int signum);
void			remove_echoctl(void);
void			active_echoctl(void);
/*-------Syntax errors-------*/
int				handle_special_char(t_token *head);
int				get_syntax_error_status(t_token *lst_tokens);
void			get_syntax_errors(t_shell *shell);

/*-------Structs-------*/
void			init_all_structs(t_shell *shell, char **envp);
void			clear_args_lst(t_arg **lst);
void			clear_redirs_lst(t_redir **lst);
void			clear_cmd(t_cmd *cmd);
void			clear_ast(t_ast **ast);
void			free_tab_chars(char **tab);
void			clear_env_lst(t_env **env);
void			clear_lexer_tmp(t_lexer *lexer);
void			clear_shell(t_shell *shell);
void			free_child_and_exit(t_shell *shell, char **env_array, \
				int exit_code);
void			close_files(t_cmd *cmd);
void			free_and_exit(t_shell *shell, int exit_code);
void			close_all_command_fds(t_ast *node);
void			close_all_fds_and_pipes(t_shell *shell);

/*-------Lexer-------*/
t_token			*ft_lexer(char *input, t_shell *shell);
void			process_normal_state(t_lexer *lexer);
void			set_to_join(t_lexer *lexer);
int				is_pos_char_operator(t_lexer *lexer, int position);
void			write_error_missing_quote(t_lexer *lexer, char quote_char);

/*-------Token-------*/
void			create_token(t_lexer *lexer, bool to_join);
void			add_char(t_list **tmp_token, char c);
void			clear_tokens_lst(t_token **lst);
t_token_type	determine_token_type(t_lexer *lexer);
char			*create_token_value(t_lexer *lexer);

/*-------AST-------*/
t_ast			*parse_pipe(t_shell *shell, t_token **tokens);
t_ast			*parse_cmd(t_token **tokens, t_shell *shell);
t_cmd			*set_cmd_name(t_cmd *new, t_arg *arg);
void			ft_lstadd_args(t_arg **args, t_arg *new);
void			create_args_lst(t_arg **args, t_token *token, t_shell *shell);
void			lst_args_to_array(t_cmd *cmd, t_arg **args);
void			get_exp_value(t_token *token, t_shell *shell, t_arg *new_arg);
void			create_redir_lst(t_token *token, t_cmd *cmd);

/*-------Builtin-------*/
/* env */
t_env			*get_env(char **envp);
void			ft_lstadd_back_env(t_env **lst, t_env *new);
int				builtin_env(t_env *env);
/* unset */
int				builtin_unset(t_env **env, char **args);
/* export */
int				builtin_export(t_env **env, char **args);
int				value_to_append(char *input);
char			*get_input_value(char *input);
char			*get_input_key(char *input);
t_env			*get_node(t_env **head, char *key);
t_env			*create_new_env_node(t_env *new, char *input, char *key);
int				print_env_export(t_env *env);
/* expand */
int				get_char_index(char *input, char c);
int				get_var_end_index(char *input);
char			*builtin_expand(char *input, t_shell *shell, char *result);
/* pwd */
int				builtin_pwd(void);
/* cd */
int				builtin_cd(char **args, t_env *env);
/* echo */
int				builtin_echo(char **args);
/* exit */
int				builtin_exit(t_shell *shell, char **args, \
				int saved_in, int saved_out);

/*-------Execution-------*/
/* Shell */
void			init_shell(t_shell *shell, char **envp);
void			reset_exec(t_shell *shell);
int				main_loop(t_shell *shell);

/* builtins */
int				is_a_builtin(char *name);
bool			is_parent_builtin(char *name);
int				exec_builtin_simple(t_cmd *cmd, t_shell *shell, \
				int fd_i, int fd_o);
int				exec_builtin_in_parent(t_cmd *cmd, t_shell *shell, \
				int fd_i, int fd_o);
int				execute_builtins(t_cmd *cmd, t_shell *shell);
void			close_backups(int saved_in, int saved_out);
int				execute_parent_builtins(t_cmd *cmd, t_shell *shell, \
				int saved_in, int saved_out);
/* others cmds */
char			**lst_env_to_array(t_env *env);
int				cmd_not_found(t_cmd *cmd);
int				prepare_cmd(t_cmd *cmd, t_env *env);
int				execute_cmd(t_ast *node, t_shell *shell, int fd_i, int fd_o);
void			execute_ast(t_ast *node, t_shell *shell, int fd_i, int fd_o);
void			execution(t_ast *ast, t_shell *shell);
void			close_all_pipes(t_shell *shell);
int				get_exit_code(int status);
/* redir & heredoc */
int				get_unique_id(void);
int				open_infile(char *infile);
int				open_outfile(char *outfile, t_token_type type);
int				prepare_redirections(t_cmd *cmd);
void			manage_dup(t_cmd *cmd, int fd_i, int fd_o);
void			handle_all_heredocs(t_ast *node, t_shell *shell);
void			close_prev_fd_heredoc(t_ast *node);
void			cleanup_heredoc_on_error(char *tmp_file_name, int fd_tmp, \
				t_shell *shell);
int				open_and_create_here_doc(char *tmp_file_name);
char			*get_file_name(void);
pid_t			execute_child_heredoc(t_shell *shell, char *limiter, \
				int fd_heredoc, char *file);
void			ft_close_fd(int *fd);

#endif
