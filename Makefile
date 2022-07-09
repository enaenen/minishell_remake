# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: wchae <wchae@student.42.fr>                +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/06/11 17:09:27 by wchae             #+#    #+#              #
#    Updated: 2022/07/09 19:56:30 by wchae            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME			=	minishell

READLINE_DIR	=	readline
READLINE_LIB	=	$(READLINE_DIR)
READLINE		=	$(READLINE_DIR)/libreadline.a

CC				=	gcc
CFLAGS			=	-Wall -Werror -Wextra -g #-fsanitize=address

INCS			=	-Ilibft -I.

SRCS_DIR		=	srcs
SRCS_NAME		=	main.c\
					builtin_echo.c\
					builtin_pwd.c\
					builtin_exit.c\
					builtin_cd.c\
					builtin_env.c\
					builtin_export.c\
					builtin_unset.c\
					cmd_list.c\
					expand.c\
					handle_builtin.c\
					handle_execute_pipe.c\
					handle_execute.c\
					handle_redirect.c\
					handle_signal.c\
					heredoc.c\
					settings.c\
					split_token.c\
					utils_error.c\
					utils_env.c\
					utils_lst.c\
					utils_quote.c\
					utils_quote2.c\
					test_print.c

SRCS			=	$(addprefix $(SRCS_DIR)/, $(SRCS_NAME))

OBJS_DIR		=	objs
OBJS			=	$(addprefix $(OBJS_DIR)/, $(SRCS_NAME:.c=.o))

LIB_DIR			=	libft
LIB_NAME		=	ft
LIB				=	$(addprefix $(LIB_DIR)/, libft.a)

$(NAME)			:	$(LIB) $(OBJS) $(READLINE)
					$(CC) $(CFLAGS) $(INCS) -L$(LIB_DIR) -l$(LIB_NAME) -L$(READLINE_DIR) -lreadline -lhistory -lncurses $(OBJS) -o $@

$(READLINE)		:
					@cd $(READLINE_DIR); ./configure
					@$(MAKE) -C $(READLINE_DIR) all

$(LIB)			:
					$(MAKE) -C $(LIB_DIR) all

$(OBJS_DIR)/%.o	:	$(SRCS_DIR)/%.c
					mkdir -p $(OBJS_DIR)
					$(CC) $(CFLAGS) $(INCS) $(CPPFLAGS) -c $< -o $@

all				:	$(NAME)

clean			:
					$(MAKE) -C $(LIB_DIR) clean
					$(MAKE) -C $(READLINE_DIR) clean
					rm -rf $(OBJS_DIR)

fclean			: 	clean
					rm -rf $(LIB)
					rm -rf $(NAME) $(OBJS_DIR)

re 				:	fclean
					$(MAKE)

.PHONY			: 	all clean fclean re
