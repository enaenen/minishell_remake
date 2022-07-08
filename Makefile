# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: seseo <seseo@student.42.fr>                +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/06/11 17:09:27 by wchae             #+#    #+#              #
#    Updated: 2022/07/08 22:01:38 by seseo            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME			=	minishell

READLINE_DIR	=	readline
READLINE_LIB	=	$(READLINE_DIR)
READLINE		=	$(READLINE_DIR)/libreadline.a

CC				=	gcc
CFLAGS			=	-Wall -Werror -Wextra -g -fsanitize=address,undefined

INCS			=	-Ilibft -I.

SRCS_DIR		=	srcs
SRCS_NAME		=	main.c\
					sig_handler.c\
					exe_handler.c\
					heredoc.c\
					lst_utils.c\
					settings.c\
					gnl.c\
					builtin_echo.c\
					builtin_pwd.c\
					builtin_exit.c\
					builtin_cd.c\
					builtin_env.c\
					builtin_export.c\
					builtin_unset.c\
					env_utils.c

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

re 				:	#fclean
					$(MAKE)

.PHONY			: 	all clean fclean re