/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wchae <wchae@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/11 17:08:37 by wchae             #+#    #+#             */
/*   Updated: 2022/07/11 01:13:51 by wchae            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <fcntl.h>
# include <sys/wait.h>
# include <sys/errno.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <sys/ioctl.h>
# include <sys/ttydefaults.h>
# include <termios.h>
# include <signal.h>
# include <dirent.h>
# include <curses.h>
# include <errno.h>
# include "libft.h"

# define ERROR -1
# define S_QUOTE 1
# define D_QUOTE 2


extern char			**environ;
int					g_status;

typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}	t_env;

typedef t_env		t_token;
typedef t_env		t_redir;

typedef struct s_cmd
{
	t_token			*tokens;
	t_redir			*redir;
	struct s_cmd	*next;
}	t_cmd;

typedef struct s_set
{
	struct termios	org_term;
	struct termios	new_term;
}	t_set;

void	init_set(t_set *set, t_env **env);
int		fd_print_err(char *msg);
int		error_msg(char *msg);
int		error_msg_cmd_not_found(char *msg);
void	restore_fd(int backup_io[2]);
void	backup_fd(int backup_io[2]);

/* SIGNAL */
void	sig_readline(int sig);
void	sig_exec(int sig);
void	sig_here_doc(int sig);
void	sig_here_doc_child(int sig);

/* PARSING & CHECK TOKEN */
int		split_token(char *input, t_token **token);
int		process_heredoc(t_token *token);
int		find_valid_quot_point(char *data, int start);

/* EXPAND */
char	*expand_str(t_env *env, char *str);
int		expand_here_doc(t_env *env, t_redir *redir);
void	expand_tokens(t_env *env, t_token *tokens);
int		is_quote(char c);
char	**split_skip_quote(char *str);
char	*skip_quote(t_buffer *buf, char *data, int q_flag);
char	*skip_quote_2(t_buffer *buf, char *data, int q_flag);
char	*rm_quote(char *data);
void	rm_quote_tokens(t_token *tokens);

/* LIST, MAP UTILS */
void	env_lstadd_back(t_env **lst, char *key, char *value);
void	env_lstadd_back_node(t_env **lst, t_env	*node);
t_env	*env_dup_check(t_env *env_list, char *new_key);
void	del_env_lst(t_env *lst);
void	del_cmd_list(t_cmd *cmd);
void	cmd_lstadd_back(t_cmd **lst, t_token *tokens);
char	**tokens_to_strs(t_token *tokens);
char	**lst_to_strs(t_list *lst);
t_cmd	*make_cmd_list(t_token *tokens);

/* ENVIRONMENT SETTINGS */
void	set_env_node(t_env **env, char *key, char *val);
t_env	*find_env_node(t_env *env_list, char *key);
char	**get_env_list(t_env **env_list);
char	*read_key(t_env *env_list, char *key);
char	*replace_env_val(t_env *env, t_buffer *buf, char *data);

/* REDIRECTION */
int		apply_redir(t_env *env, t_cmd *cmd);
void	set_redir(t_cmd *cmd);


/* EXECUTE FUNCTIONS */
int		check_builtin_cmd(t_token *tokens);
int		execute_builtin_cmd(t_env *env_list, t_cmd *cmd, char **exe);
int		execute_builtin_cmd_pipe(t_env *env_list, t_cmd *cmd, char **exe);
int		do_cmd(t_env *env, t_cmd *cmd);
int		do_exec_function(t_env *env, t_token *tokens);
int		do_pipe(t_env *env, t_cmd *cmd, int n_pipe);
int		do_pipe_cmd(t_env *env, t_cmd *cmd);
int		do_actual_path_cmd(t_cmd *cmd, char **args, char **envp);
int		do_cmd_child(t_env *env, t_cmd *cmd);

/* BUILTIN COMMANDS */
int		ft_pwd(void);
int		ft_echo(char **buf);
int		ft_exit(char **buf);
int		ft_exit_pipe(char **buf);
int		ft_cd(char **buf, t_env *env_list);
int		ft_env(t_env *env_list);
int		ft_export(char **buf, t_env *env_list);
int		ft_unset(char **buf, t_env **env_list);

/* for TESTs */
void	print_env_list(t_env *env);
void	ft_lstprint(t_list *lst);
void	print_cmd(t_cmd *cmd);
void	print_dchar(char **str);


void	ft_kill_exit(void);
#endif
