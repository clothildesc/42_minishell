# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: cscache <cscache@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/06/11 18:25:47 by cscache           #+#    #+#              #
#    Updated: 2025/09/17 10:31:46 by cscache          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = cc
CFLAGS = -Wall -Wextra -Werror
LDFLAGS = -lreadline

NAME = minishell

SRCS_DIR = sources
OBJS_DIR = obj
LIBFT_DIR = libft

LIBFT = $(LIBFT_DIR)/libft.a
INCLUDES = -I includes -I $(LIBFT_DIR)
HEADER = includes/minishell.h

SRCS = $(SRCS_DIR)/main.c

BLTN_DIR = builtin
BUILTIN = $(SRCS_DIR)/$(BLTN_DIR)/builtin_echo.c\
			$(SRCS_DIR)/$(BLTN_DIR)/builtin_env.c\
			$(SRCS_DIR)/$(BLTN_DIR)/builtin_exit.c\
			$(SRCS_DIR)/$(BLTN_DIR)/builtin_expand.c\
			$(SRCS_DIR)/$(BLTN_DIR)/builtin_expand_utils.c\
			$(SRCS_DIR)/$(BLTN_DIR)/builtin_export_utils.c\
			$(SRCS_DIR)/$(BLTN_DIR)/builtin_export_utils_2.c\
			$(SRCS_DIR)/$(BLTN_DIR)/builtin_export.c\
			$(SRCS_DIR)/$(BLTN_DIR)/builtin_pwd.c\
			$(SRCS_DIR)/$(BLTN_DIR)/builtin_cd.c\
			$(SRCS_DIR)/$(BLTN_DIR)/builtin_unset.c

EXC_DIR = exec
EXEC = $(SRCS_DIR)/$(EXC_DIR)/exec_ast.c\
		$(SRCS_DIR)/$(EXC_DIR)/exec_builtins.c\
		$(SRCS_DIR)/$(EXC_DIR)/exec_builtins_utils.c\
		$(SRCS_DIR)/$(EXC_DIR)/exec_cmd.c\
		$(SRCS_DIR)/$(EXC_DIR)/exec_cmd_utils.c\
		$(SRCS_DIR)/$(EXC_DIR)/exec_cmd_utils_2.c\
		$(SRCS_DIR)/$(EXC_DIR)/exec_heredoc.c\
		$(SRCS_DIR)/$(EXC_DIR)/exec_heredoc_utils.c\
		$(SRCS_DIR)/$(EXC_DIR)/exec_heredoc_utils_2.c\
		$(SRCS_DIR)/$(EXC_DIR)/exec_redirs.c\
		$(SRCS_DIR)/$(EXC_DIR)/exec_redirs_utils.c\
		$(SRCS_DIR)/$(EXC_DIR)/exec_shell.c\
		$(SRCS_DIR)/$(EXC_DIR)/exec_utils.c

LXR_DIR = lexer
LEXER = $(SRCS_DIR)/$(LXR_DIR)/lexer.c\
		$(SRCS_DIR)/$(LXR_DIR)/lexer_check_errors.c\
		$(SRCS_DIR)/$(LXR_DIR)/lexer_check_errors_utils.c\
		$(SRCS_DIR)/$(LXR_DIR)/lexer_tokens.c\
		$(SRCS_DIR)/$(LXR_DIR)/lexer_tokens_utils.c\
		$(SRCS_DIR)/$(LXR_DIR)/lexer_utils.c

PRSG_DIR = parsing
PARSING = $(SRCS_DIR)/$(PRSG_DIR)/parsing_pipe.c\
			$(SRCS_DIR)/$(PRSG_DIR)/parsing_cmd.c\
			$(SRCS_DIR)/$(PRSG_DIR)/parsing_cmd_utils.c\
			$(SRCS_DIR)/$(PRSG_DIR)/parsing_cmd_utils_2.c\
			$(SRCS_DIR)/$(PRSG_DIR)/parsing_cmd_utils_3.c

SGN_DIR = signals
SIGNALS = $(SRCS_DIR)/$(SGN_DIR)/signals_setup.c\
			$(SRCS_DIR)/$(SGN_DIR)/signals_echoctl.c

STRCT_DIR = structs
STRUCTS = $(SRCS_DIR)/$(STRCT_DIR)/structs_init.c\
			$(SRCS_DIR)/$(STRCT_DIR)/get_env.c\
			$(SRCS_DIR)/$(STRCT_DIR)/close_files.c\
			$(SRCS_DIR)/$(STRCT_DIR)/free_cmds.c\
			$(SRCS_DIR)/$(STRCT_DIR)/free_structs.c\
			$(SRCS_DIR)/$(STRCT_DIR)/free_utils.c

SRCS_FILES = $(SRCS) $(BUILTIN) $(EXEC) $(LEXER) $(PARSING) $(SIGNALS) $(STRUCTS)

OBJS = $(patsubst $(SRCS_DIR)/%.c,$(OBJS_DIR)/%.o,$(SRCS_FILES))

all: $(NAME)

$(NAME): $(LIBFT) $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -L$(LIBFT_DIR) -lft $(LDFLAGS) -o $(NAME)

$(OBJS_DIR)/%.o: $(SRCS_DIR)/%.c $(HEADER)
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(LIBFT):
	make -C $(LIBFT_DIR)

clean:
	make -C $(LIBFT_DIR) clean
	rm -rf $(OBJS_DIR)

fclean: clean
	make -C $(LIBFT_DIR) fclean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
