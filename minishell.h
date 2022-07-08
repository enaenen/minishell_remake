/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seseo <seseo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/11 17:08:37 by wchae             #+#    #+#             */
/*   Updated: 2022/07/08 18:41:02 by seseo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <fcntl.h>
# include <sys/wait.h>
# include <sys/errno.h>
# include <string.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <sys/ioctl.h>
# include <termios.h>
# include <signal.h>
# include <dirent.h>
# include <curses.h>
# include "libft.h"

# define ERROR -1
# define S_QUOTE 1
# define D_QUOTE 2

// GNL ASSETS
# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 32
# endif

# ifndef OPEN_MAX
#  define OPEN_MAX 256
# endif

# define SUCCESS 1
# define END_OF_FILE 0
// GNL ASSETS END

extern char			**environ;
int					g_status;

typedef struct s_list
{
	char			*data;
	struct s_list	*next;
}					t_list;

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
	int				org_stdin;
	int				org_stdout;
}	t_set;

typedef struct s_proc
{
	t_env	*env_list;
	t_token	*tokens;
	int		infile;
	int		outfile;
	int		pipe_flag;
	int		status;
}	t_proc;

int		error_msg(char *msg);
void	ft_sig_handler(int status);
void	sig_here_doc(int sig);
int		process_heredoc(t_token *token);
void	sig_here_doc_child(int sig);
void	sig_exec(int sig);
void	sig_readline(int sig);

/** LIST UTILS **/
t_list	*ft_lstlast(t_list *lst);
void	ft_lstadd_back(t_list **lst, t_list *new);
t_list	*ft_lstnew(void *data);
void	ft_lstclear(t_list **lst, void (*del)(void *));
int		ft_lstsize(t_list *lst);

void	env_lstadd_back(t_env **lst, char *key, char *value);
void	env_lstadd_back_node(t_env **lst, t_env	*node);
void	del_env_lst(t_env *lst);
void	cmd_lstadd_back(t_cmd **lst, t_token *tokens);

void	set_env_node(t_env **env, char *key, char *val);
t_env	*find_env_node(t_env *env_list, char *key);

void	init_set(t_set *set, t_env **env);
void	init_set2(t_set	*set, char ***envp, t_env *env);

int		check_builtin_cmd(t_list *cmd);
void	execute_builtin_cmd(t_proc *proc, char **exe);

/* BUILTIN COMMANDS */
void	ft_pwd(void);
int		ft_echo(char **buf);
void	ft_exit(char **buf);
void	ft_cd(char **buf, t_env *env_list);
int		ft_env(t_env *env_list);
int		ft_cd2(t_proc *proc, char **cmd);
void	ft_export(char **buf, t_env *env_list, char **splits, t_env *temp);
void	ft_unset(char **buf, t_env **env_list);

char	**get_env_list(t_env **env_list);
char	*get_next_line(int fd);


#endif
