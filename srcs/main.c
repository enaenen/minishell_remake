/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seseo <seseo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/11 16:28:59 by wchae             #+#    #+#             */
/*   Updated: 2022/07/09 16:17:43 by seseo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <sys/ttydefaults.h>
/**==============LinkedList============**/

void	ft_lstprint(t_list *lst)
{
	if (!lst)
		return ;
	while (lst)
	{
		ft_putstr(lst->data);
		ft_putstr("\n");
		lst = lst->next;
	}

}
/*========== LinkedList END===========*/


// void	reset_set(t_set *set)
// {
// 	tcsetattr(STDIN_FILENO, TCSANOW, &set->org_term);
// }

/* UTIL */
int	error_msg(char *msg)
{
	g_status = 2;
	write(2, "bash: ", 6);
	if (!msg)
		write(2, "syntax error near unexpected token 'newline'", 44);
	else if (msg[0] == '|' || msg[0] == '<' || msg[0] == '>')
	{
		write(2, "syntax error near unexpected token '", 36);
		write(2, msg, ft_strlen(msg));
		write(2, "'\n", 2);
		return (ERROR);
	}
	else
	{
		g_status = 127;
		write(2, msg, ft_strlen(msg));
		if (ft_strncmp(strerror(errno), "Bad address", 12) == 0)
			write(2, ": command not found", 19);
		else
		{
			write(2, ": ", 2);
			write(2, strerror(errno), ft_strlen(strerror(errno)));
		}
	}
	write(2, "\n", 1);
	return (g_status);
}
/* UTIL END*/

/* SPLIT TOKEN */
/** QUOTE**/
int		 find_valid_quot_point(char *data, int start)
{
	int find;

	find = start + 1;
	//닫는게 있다면 찾은지점 index return
	while (data[find] && data[start] != data[find])
		find++;
	if (data[find])
		return (find);
	//없다면 시작지점 return
	return (start);
}

/**
 *
 * 쪼개는 작업
 *  "echo abc > a.txt"
 * 	"echo abc >" AND "a.txt"
 **/

int		split_redirection_token(char *input, int i, t_token **token)
{
	char	*tmp;
	int		save;

	save = i;
	if (i != 0)
	{
		tmp = ft_strntrim(input, " ", i);
		if (!tmp)
			return (error_msg("malloc"));
		env_lstadd_back(token, tmp, NULL);
		input = &input[i];
		i = 0;
	}
	while (input[i] == '<' || input[i] == '>')
		i++;
	tmp = ft_strntrim(input, " ", i);
	if (!tmp)
		return (error_msg("malloc"));
	env_lstadd_back(token, tmp, NULL);
	return (i + save);
}

int	split_space_token(char *input, int i, t_token **token)
{
	char	*tmp;

	if (i != 0)
	{
		tmp = ft_strntrim(input, " ", i);
		if (!tmp)
			return (error_msg("malloc"));
		env_lstadd_back(token, tmp, NULL);
	}
	while (input[i] == ' ')
		i++;
	return (i);
}
/**
 *
 * 쪼개는 작업
 *  "echo abc | ls -l"
 * 	"echo abc" AND "|"" AND "ls -l"
 **/

int		split_pipe_token(char *input, int i, t_token **token)
{
	char	*tmp;

	if (i != 0)
	{
		tmp = ft_strntrim(input, " ", i);
		if (!tmp)
			return (error_msg("malloc"));
		env_lstadd_back(token, tmp, NULL);
	}
	tmp = ft_strdup("|");
	if (!tmp)
		return (error_msg("malloc"));
	env_lstadd_back(token, tmp, NULL);
	return (i + 1);
}

int		split_rest_token(char *input, t_token **token)
{
	char	*tmp;

	if (input[0])
	{
		tmp = ft_strntrim(input, " ", ft_strlen(input));
		if (!tmp)
			return (error_msg("malloc"));
		env_lstadd_back(token, tmp, NULL);
	}
	return (TRUE);
}

int		split_token(char *input, t_token **token)
{
	int	i;

	i = -1;
	while (input[++i])
	{
		if (input[i] == '\"' || input[i] == '\'')
		{
			i = find_valid_quot_point(input, i);
			continue ;
		}
		else if (input[i] == '<' || input[i] == '>')
			i = split_redirection_token(input, i, token);
		else if (input[i] == ' ')
			i = split_space_token(input, i , token);
		else if (input[i] == '|')
			i = split_pipe_token(input, i, token);
		else
			continue ;
		if (i == ERROR)
			return (ERROR);
		input = &input[i];
		i = -1;
	}
	return (split_rest_token(input, token));
}

/* SPLIT TOKEN END */

/* CHECK TOKEN */

int		check_token(t_token	*token)
{
	int	i;

	i = 0;
	while (token)
	{
		// | 나 ||밖에없을때
		if (token->key[0] == '|' && (i == 0
			|| !token->next || token->next->key[0] == '|'))
			return (error_msg("|"));
		// < | > 관련 에러처리
		else if (token->key[0] == '<' || token->key[0] == '>')
		{
			if (token->key[1] && token->key[0] != token->key[1])
				return (error_msg(&token->key[1]));
			else if (2 < ft_strlen(token->key))
				return (error_msg(&token->key[2]));
			else if (!token->next)
				return (error_msg(NULL));
			else if (token->next->key[0] == '<'
				|| token->next->key[0] == '>'
				|| token->next->key[0] == '|')
				return (error_msg(token->next->key));
		}
		i++;
		token = token->next;
	}
	return (TRUE);
}

/* PIPE TOKEN */

/** IN PIPE _ TOKEN PROCESS **/
char	*ms_strtrim(char *data, int start, int end)
{
	int		i;
	int		j;
	char	*ret;

	ret = (char *)malloc(end * sizeof(char));
	if (!ret)
		return (NULL);
	i = -1;
	j = 0;
	while (data[++i] && i < end)
	{
		if (i != start)
			ret[j++] = data[i];
	}
	ret[j] = '\0';
	return (ret);
}

// data를 받아 data +1 부터 $ 토큰을 찾음
int		find_env_var_token(char *data, int start, int end)
{
	int	find;

	find = start + 1;
	while (data[find] && find < end && data[find] != '$')
		find++;
	if (find == end)
		return (FALSE);
	return (TRUE);
}
/** env 해석
 * read_key env 해석 하여 key의 value 값 가져옴
 * strjoin 으로 합치기
 * **/

char	*read_key(t_env *env_list, char *key)
{
	while (env_list)
	{
		if (!ft_strcmp(env_list->key, key))
			return (ft_strdup(env_list->value));
		env_list = env_list->next;
	}
	return (NULL);
}


char	*parse_pre_env_var(char *data, int start, char *new_data)
{
	char	*org_data;
	char	*tmp;

	org_data = new_data;
	tmp = ft_strndup(data, start);
	if (!tmp)
		return (ft_free(org_data));
	new_data = ft_strjoin(new_data, tmp);
	ft_free(tmp);
	ft_free(org_data);
	return (new_data);

}


int		find_valid_env_var_point(char *data)
{
	int	index;
	if (data[0] == '?')
		return (1);
	index = 0;
	while (data[index] && (ft_isalnum(data[index]) || data[index] == '_'))
		index++;
	return (index);
}


char	*expand_env_var(t_proc *proc, char *data, int start, char **new_data)
{
	char	*tmp;
	char	*get_env;

	*new_data = parse_pre_env_var(data, start, *new_data);
	if (!(*new_data))
		return (NULL);
	data = &data[start + 1];
	//$ 이후에 num, alnum or _ 일때
	get_env = ft_strndup(data, find_valid_env_var_point(data));
	data = &data[find_valid_env_var_point(data)];
	if (!get_env)
		return (ft_free(*new_data));
	tmp = get_env;
	if (get_env[0] == '?')
		get_env = ft_itoa(g_status);
	else
		get_env = read_key(proc->env_list, get_env);
	ft_free(tmp);
	tmp = *new_data;
	*new_data = ft_strjoin(*new_data, get_env);
	ft_free(tmp);
	ft_free(get_env);
	if (!(*new_data))
		return (NULL);
	return (data);
}

char	*expand_int_quot_utils(t_proc *proc, char *data, char **new_data)
{
	int i;
	i = -1;
	while (data[++i])
	{
		if (data[i]== '$')
		{
			data = expand_env_var(proc, data, i ,new_data);
			if (!data)
				return (ft_free(*new_data));
			i = -1;
		}
	}
	return (data);
}

char	*expand_in_quot_utils(t_proc *proc, char *data, char **new_data)
{
	int i;

	i = -1;
	while (data[++i])
	{
		if (data[i] == '$')
		{
			data = expand_env_var(proc, data, i, new_data);
			if (!data)
				return (ft_free(*new_data));
			i = -1;
		}
	}
	return (data);
}

char	*expand_in_quot_env_var(t_proc *proc, char *data, int start, int end)
{
	char	*new_data;
	char	*tmp;
	char	*tmp2;

	new_data = ft_strndup(data, start);
	if (!new_data)
		return (NULL);
	data = &data[start + 1];
	data = ft_strndup(data, end - start - 1);
	if (!data)
		return (ft_free(new_data));
	tmp2 = data;
	data = expand_in_quot_utils(proc, data, &new_data);
	if (!data)
		return (ft_free(tmp2));
	tmp = new_data;
	new_data = ft_strjoin(new_data, data);
	ft_free(tmp2);
	ft_free(tmp);
	return (new_data);
}
/*QUOT 처리*/
//data 는 quot 뒤엣부분
char	*del_big_quot(t_proc *proc, char *data, int start, char **new_data)
{
	int		end;
	char	*org_data;
	char	*tmp;

	org_data = *new_data;
	tmp = NULL;
	end = find_valid_quot_point(data, start);
	if (find_env_var_token(data, start, end) == TRUE)
		tmp = expand_in_quot_env_var(proc, data, start, end);
	else
		tmp = ms_strtrim(data, start, end);
	if (!tmp)
		return (ft_free(org_data));
	*new_data = ft_strjoin(*new_data, tmp);
	ft_free(org_data);
	ft_free(tmp);
	if (!(*new_data))
		return (NULL);
	data = &data[end + 1];
	return (data);
}

char	*del_small_quot_token(char *data, int start, char **new_data)
{
	int		end;
	char	*tmp;
	char	*org_data;

	org_data = *new_data;
	end = find_valid_quot_point(data, start);
	tmp = ms_strtrim(data, start, end);
	if (!tmp)
		return (NULL);
	*new_data = ft_strjoin(*new_data, tmp);
	ft_free(org_data);
	ft_free(tmp);
	if (!(*new_data))
		return (NULL);
	data = &data[end + 1];
	return (data);
}
/** END IN PIPE _ TOKEN PROCESS **/
/** expand_var**/

char	*expand_in_quot_uitls(t_proc *proc, char *data, char **new)
{
	int	i;

	i = -1;
	while (data[++i])
	{
		if (data[i] == '$')
		{
			data = expand_env_var(proc, data, i , new);
			if (!data)
				return(ft_free(*new));
			i = -1;
		}
	}
	return (data);
}

int	is_quote(char c)
{
	if (c == '\'')
		return (S_QUOTE);
	else if (c == '"')
		return (D_QUOTE);
	return (0);
}

char	*skip_quote(t_buffer *buf, char *data, int q_flag)
{
	data++;
	while (is_quote(*data) != q_flag)
	{
		add_char(buf, *data);
		data++;
	}
	return (data);
}

char	*skip_quote_2(t_buffer *buf, char *data, int q_flag)
{
	add_char(buf, *data);
	data++;
	while (is_quote(*data) != q_flag)
	{
		add_char(buf, *data);
		data++;
	}
	add_char(buf, *data);
	return (data);
}

char	*rm_quote(char *data)
{
	t_buffer	*buf;
	char		*str;

	buf = create_buf();
	while (*data)
	{
		if (is_quote(*data))
			data = skip_quote(buf, data, is_quote(*data));
		else
			add_char(buf, *data);
		data++;
	}
	str = put_str(buf);
	del_buf(buf);
	return (str);
}

char	*replace_env_val(t_env *env, t_buffer *buf, char *data)
{
	char	*key;
	char	*str;
	int		i;

	i = 1;
	if (*(data + 1) == '?')
		str = ft_itoa(g_status);
	else if (*(data + 1) == 0)
		str = ft_strdup("$");
	else if (ft_isdigit(*(data + 1)))
		str = NULL;
	else
	{
		while (data[i] && (ft_isalnum(data[i]) || data[i] == '_'))
			i++;
		if (i == 1)
			str = ft_substr(data, 0, 2);
		else
		{
			key = ft_substr(data, 1, i);
			str = read_key(env, key);
			free(key);
		}
	}
	add_str(buf, str);
	free(str);
	return (data);
}

//malloc error
char	*expand_data(t_env *env_list, char *data)
{
	t_buffer	*buf;
	char		*str;

	buf = create_buf();
	while (*data)
	{
		if (is_quote(*data) == S_QUOTE)
			data = skip_quote_2(buf, data, S_QUOTE);
		else if (*data == '$')
			data = replace_env_val(env_list, buf, data);
		else
			add_char(buf, *data);
		if (data == NULL)
			break ;
		data++;
	}
	str = put_str(buf);
	del_buf(buf);
	return (str);
}

char	*skip_space(char *str)
{
	while (*str)
	{
		if (*str != ' ')
			break ;
		str++;
	}
	return (str);
}

char	**lst_to_strs(t_list *lst)
{
	t_list	*tmp;
	char	**strs;
	int		i;

	i = 0;
	tmp = lst;
	while (tmp)
	{
		i++;
		tmp = tmp->next;
	}
	strs = malloc(sizeof(char *) * (i + 1));
	strs[i] = NULL;
	i = 0;
	while (lst)
	{
		strs[i++] = lst->data;
		lst = lst->next;
	}
	return (strs);
}

char	**split_skip_quote(char *str)
{
	t_buffer	*buf;
	t_list		*tmp_lst;
	char		**strs;

	tmp_lst = NULL;
	buf = create_buf();
	while (*str)
	{
		if (is_quote(*str))
			str = skip_quote_2(buf, str, is_quote(*str));
		else if (*str != ' ')
			add_char(buf, *str);
		else
		{
			str = skip_space(str);
			ft_lstadd_back(&tmp_lst, ft_lstnew(put_str(buf)));
			if (*str == 0)
				break ;
		}
		str++;
	}
	strs = lst_to_strs(tmp_lst);
	ft_lstclear(&tmp_lst, free);
	del_buf(buf);
	return (strs);
}

// >>  << < > 일때 리다이렉션 처리 --> apply redirection
// int		parse_std_inout_redirection(t_proc *proc, t_token *tokens, char *tmp)
// {
// 	if (ft_strncmp(tokens->key, "<<", 3) == 0)
// 		env_lstadd_back(&proc->limiter, tmp, NULL));
// 	if (ft_strncmp(tokens->key, "<", 3) == 0)
// 	{
// 		proc->infile = open(tmp, O_RDONLY);
// 		if (proc->infile < 0)
// 		{
// 			error_msg(tmp);
// 			ft_free(tmp);
// 			return (ERROR);
// 		}
// 		dup2(proc->infile, STDIN_FILENO);
// 	}
// 	if (ft_strncmp(tokens->key, ">>", 3) == 0)
// 		proc->outfile = open(tmp, O_WRONLY | O_CREAT | O_APPEND, 0644);
// 	else if (ft_strncmp(tokens->key, ">", 2) == 0)
// 		proc->outfile = open(tmp, O_WRONLY | O_CREAT | O_TRUNC, 0644);
// 	if (proc->outfile < 0)
// 	{
// 			error_msg(tmp);
// 			ft_free(tmp);
// 			return (ERROR);
// 	}
// 	return (TRUE);
// }

// int		parse_data(t_proc *proc, t_token *tokens)
// {
// 	char	*tmp;
// 	while (tokens)
// 	{
// 		if (tokens->key[0] == '<' || tokens->key[0] == '>')
// 		{
// 			tmp = expand_data(proc, tokens->next->key);
// 			if (!tmp)
// 				return (error_msg("malloc"));
// 			else if (parse_std_inout_redirection(proc, tokens, tmp) == ERROR)
// 				return (ERROR);
// 			ft_free(tmp);
// 			tokens = tokens->next;
// 		}
// 		else
// 		{
// 			tmp = expand_data(proc, tokens->key);
// 			if (!tmp)
// 				return (error_msg("malloc"));
// 			ft_lstadd_back(&proc->cmd, ft_lstnew(tmp));
// 		}
// 		tokens = tokens->next;
// 	}
// 	return (TRUE);
// }

char	**split_cmd(t_list *cmd)
{
	char	**exe;
	int		size;
	int		i;

	size = ft_lstsize(cmd);
	exe = (char **)malloc((size + 1) * sizeof(char *));
	if (!exe)
		return (NULL);
	i = 0;
	while (i < size)
	{
		exe[i] = cmd->data;
		cmd = cmd->next;
		i++;
	}
	exe[i] = NULL;
	return (exe);
}

char	*find_path(char *cmd, char **env_list, int i)
{
	char	*path;
	char	**paths;
	char	*tmp;
	while (env_list[i] && ft_strnstr(env_list[i], "PATH=", 5) == NULL)
	{
		// printf("envlist = %s\n",env_list[i]);
		i++;
	}
	if (env_list[i] == NULL)
		return (cmd);
	paths = ft_split(env_list[i] + 5, ':');
	i = 0;
	while (paths[i])
	{
		path = ft_strjoin(paths[i], "/");
		tmp = path;
		path = ft_strjoin(path, cmd);
		free(tmp);
		if (access(path, F_OK) == 0)
		{
			free(paths);
			return (path);
		}
		i++;
	}
	return (cmd);
}

/**COMMAND HANDLING**/
// int		execute_cmd(t_proc *proc, t_list *cmd, int *fd, char **envp)
// {
// 	char **exe;

// 	close(fd[0]);
// 	if (0 < proc->outfile)
// 		dup2(proc->outfile, STDOUT_FILENO);
// 	else
// 		dup2(fd[1], STDOUT_FILENO);
// 	close(fd[1]);
// 	exe = split_cmd(cmd);
// 	if (!exe)
// 		return (error_msg("malloc"));
// 	if (check_builtin_cmd(proc->cmd) == TRUE)
// 		execute_builtin_cmd(proc, exe);
// 	else if (exe[0][0] == '/' || exe[0][0] == '.')
// 	{
// 		if (execve(exe[0], exe, 0) == -1)
// 			return (error_msg(exe[0]));
// 	}
// 	else if (execve(find_path(exe[0], envp, 0), exe, envp) == -1)
// 		return (error_msg(exe[0]));
// 	// ft_free_split(exe);
// 	return (0);
// }

// int		handle_cmd(t_proc *proc, t_list *cmd, char **envp)
// {
// 	int		fd[2];
// 	pid_t	pid;

// 	signal(SIGINT, &sig_exec);
// 	signal(SIGQUIT, &sig_exec);

// 	if (pipe(fd) == -1)
// 		return (error_msg("pipe"));
// 	pid = fork();
// 	if (pid == 0)
// 		exit(execute_cmd(proc, cmd, fd, envp));
// 	else if (0 < pid)
// 	{
// 		close(fd[1]);
// 		dup2(fd[0], STDIN_FILENO);
// 		close(fd[0]);
// 		close(STDIN_FILENO);
// 	}
// 	else
// 	{
// 		close(fd[0]);
// 		close(fd[1]);
// 		return (error_msg("fork"));
// 	}
// 	return (0);
// }

/**COMMAND HANDLING END **/
// void	ft_envclear(t_env **lst, void (*del)(void *))
// {
// 	t_env	*next;

// 	if (!lst || !*lst || !del)
// 		return ;
// 	while (*lst)
// 	{
// 		next = (*lst)->next;
// 		del((*lst)->key);
// 		(*lst)->key = NULL;
// 		del((*lst)->value);
// 		(*lst)->value = NULL;
// 		free(*lst);
// 		*lst = NULL;
// 		*lst = next;
// 	}
// 	*lst = NULL;
// }

// int		parse_process(t_proc *proc, char **envp)
// {
// 	if (parse_data(proc, proc->data) == TRUE && proc->cmd)
// 		handle_cmd(proc, proc->cmd, envp);
// 	ft_lstclear(&proc->limiter, free);
// 	ft_lstclear(&proc->cmd, free);
// 	ft_lstclear(&proc->data, free);
// 	// ft_envclear(&proc->env_list, free);

// 	return (TRUE);
// }

/** pasre process **/
//WIP


// int other_command(t_proc *proc, t_list *cmd)
// {
// 	pid_t	pid;
// 	char	**exe;
// 	char 	**envp;

// 	signal(SIGINT, &sig_exec);
// 	signal(SIGQUIT, &sig_exec);
// 	pid = fork();
// 	if (pid == -1)
// 		exit(EXIT_FAILURE);
// 	if (pid == 0)
// 	{
// 		envp = get_env_list(&(proc->env_list));
// 		if (0 < proc->outfile)
// 			dup2(proc->outfile, STDOUT_FILENO);
// 		exe = split_cmd(cmd);
// 		if (!exe)
// 			return (error_msg("malloc"));
// 		if (check_builtin_cmd(proc->cmd) == TRUE)
// 		{
// 			execute_builtin_cmd(proc, exe);
// 			exit(g_status);
// 		}
// 		else if (exe[0][0] == '/' || exe[0][0] == '.')
// 			proc->status = execve(exe[0], exe, envp);
// 		else
// 			proc->status = execve(find_path(exe[0], envp, 0), exe, envp);
// 		if (proc->status == -1)
// 			exit(error_msg(exe[0]));
// 	}
// 	else if (0 < pid)
// 		return (0);
// 	else
// 		return (error_msg("fork"));
// 	return (0);
// }


// int parse_last_process(t_proc *proc, t_env *env)
// {
// 	char	**exe;

// 	proc->env_list = env;
// 	exe = NULL;

// 	//data expand
// 	if (parse_data(proc, proc->data) == TRUE && proc->cmd)
// 	{
// 		// signal(SIGINT, &sig_exec);
// 		// signal(SIGQUIT, &sig_exec);
// 		if (proc->pipe_flag == FALSE && check_builtin_cmd(proc->cmd))
// 		{
// 			if (0 < proc->outfile)
// 				dup2(proc->outfile, STDOUT_FILENO);
// 			exe = split_cmd(proc->cmd);
// 			if (!exe)
// 				return (error_msg("malloc"));
// 			execute_builtin_cmd(proc, exe);
// 		}
// 		else
// 		{
// 			other_command(proc, proc->cmd);
// 		}
// 	}
// 	return (TRUE);
// }
/** parse process END **/

void	print_strs(char **strs)
{
	while (*strs)
	{
		printf("%s\n", *strs++);
	}
}

// int		parse_pipe_token(t_list *token, t_env *env)
// {
// 	char	*tmp;
// 	t_proc	proc;
// 	// char	**envp;

// 	// envp = get_env_list(&env);
// 	ft_memset(&proc, 0, sizeof(t_proc));
// 	proc.env_list = env;
// 	while (token)
// 	{
// 		ft_lstprint(token);
// 		if (token->data[0] != '|')
// 		{
// 			tmp = ft_strdup(token->data);

// 			if (!tmp)
// 				return (error_msg("malloc"));
// 			ft_lstadd_back(&proc.data, ft_lstnew(tmp));
// 		}
// 		if (token->data[0] == '|')
// 		{
// 			// parse_process(&proc);
// 			ft_memset(&proc, 0, sizeof(t_proc));
// 			proc.pipe_flag = TRUE;
// 		}
// 		if (!token->next)
// 			parse_last_process(&proc, env);
// 		token = token->next;
// 	}
// 	ft_lstclear(&proc.limiter, free);
// 	ft_lstclear(&proc.cmd, free);
// 	ft_lstclear(&proc.data, free);
// 	return (TRUE);
// }
/* END PIPE*/


/* CHECK TOKEN END */

t_token	*parse_input(char *input, t_env *env)
{
	t_token	*token;

	(void)env;
	token = NULL;
	add_history(input);
	if (split_token(input, &token) == TRUE && check_token(token) == TRUE)
	{
		g_status = process_heredoc(token);
		if (g_status)
		{
			del_env_lst(token);
			return (NULL);
		}
	}
	return (token);
	// 	parse_pipe_token(token, env);
	// 	while (0 < waitpid(-1, &g_status, 0))
	// 		continue ;
	// if (WIFEXITED(g_status))
	// 	g_status = WEXITSTATUS(g_status);
	// ft_lstclear(&token, free);
}
/**
 * SETTING
 */
// void	reset_stdio(t_set *set)
// {
// 	dup2(set->org_stdin, STDIN_FILENO);
// 	dup2(set->org_stdout, STDOUT_FILENO);
// }

/**fortest**/
void	print_env_list(t_env *env)
{
	while (env){
		printf("key = %s ",env->key);
		env = env->next;
	}
	printf("\n");
}

t_cmd	*make_cmd_list(t_env *env, t_token *tokens)
{
	t_cmd	*cmd;
	t_token	*head;
	t_token	*prev;
	t_token	*cur;

	(void)env;
	head = tokens;
	cur = tokens;
	cmd = NULL;
	while (cur)
	{
		prev = cur;
		cur = cur->next;
		if (ft_strncmp(prev->key, "|", -1) == 0)
		{
			free(prev->key);
			free(prev);
			prev = NULL;
		}
		if (cur && ft_strncmp(cur->key, "|", -1) == 0)
		{
			prev->next = NULL;
			cmd_lstadd_back(&cmd, head);
			head = cur->next;
		}
	}
	cmd_lstadd_back(&cmd, head);
	return (cmd);
}

void	set_redir(t_cmd *cmd)
{
	t_cmd	*cmd_tmp;
	t_token	*head;
	t_token	*prev;
	t_token	*cur;

	cmd_tmp = cmd;
	while (cmd_tmp)
	{
		cur = cmd_tmp->tokens;
		cmd_tmp->tokens = NULL;
		head = NULL;
		while (cur)
		{
			if (cur && (ft_strncmp(cur->key, "<", 1) == 0 || ft_strncmp(cur->key, ">", 1) == 0))
			{
				head = cur;
				cur = head->next->next;
				head->next->next = NULL;
				env_lstadd_back_node(&cmd_tmp->redir, head);
				continue ;
			}
			prev = cur;
			cur = cur->next;
			prev->next = NULL;
			env_lstadd_back_node(&cmd_tmp->tokens, prev);
		}
		cmd_tmp = cmd_tmp->next;
	}
}

int	fd_print_err(int fd)
{
	if (fd == -1)
	{
		error_msg(strerror(errno));
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

int	expand_here_doc(t_env *env, t_redir *redir)
{
	t_buffer	*buf;
	char		*expanded_str;
	int			io_heredoc[2];

	if (pipe(io_heredoc))
		exit(EXIT_FAILURE);
	buf = create_buf();
	expanded_str = expand_data(env, redir->value);
	write(io_heredoc[1], expanded_str, ft_strlen(expanded_str));
	close(io_heredoc[1]);
	return (io_heredoc[0]);
}

int	apply_redir(t_env *env, t_cmd *cmd)
{
	int		fd;
	t_redir	*redir;

	redir = cmd->redir;
	while (redir)
	{
		if(ft_strncmp(redir->key, "<", -1) == 0 || ft_strncmp(redir->key, "<<", -1) == 0)
		{
			if (ft_strncmp(redir->key, "<", -1) == 0)
				fd = open(redir->next->key, O_RDONLY);
			else
				fd = expand_here_doc(env, redir);
			if (fd_print_err(fd))
				return (EXIT_FAILURE);
			dup2(fd, STDIN_FILENO);
			close(fd);
		}
		else
		{
			if (ft_strncmp(redir->key, ">", -1) == 0)
				fd = open(redir->next->key, O_TRUNC | O_CREAT | O_WRONLY, 0777);
			else
				fd = open(redir->next->key, O_APPEND | O_CREAT | O_WRONLY, 0777);
			if (fd_print_err(fd))
				return (EXIT_FAILURE);
			dup2(fd, STDOUT_FILENO);
			close(fd);
		}
		redir = redir->next->next;
	}
	return (EXIT_SUCCESS);
}

void	print_cmd(t_cmd *cmd)
{
	while (cmd)
	{
		print_env_list(cmd->tokens);
		print_env_list(cmd->redir);
		cmd = cmd->next;
	}
}

void	backup_fd(int backup_io[2])
{
	backup_io[0] = dup(STDIN_FILENO);
	backup_io[1] = dup(STDOUT_FILENO);
}

int	do_actual_path_cmd(t_cmd *cmd, char **args, char **envp)
{
	execve(cmd->tokens->key, args, envp);
	error_msg(strerror(errno));
	return (127);
}

int	do_cmd_child(t_env *env, t_cmd *cmd)
{
	char	**envp;
	char	**args;
	char	**path;
	int		i;

	envp = get_env_list(&env);
	args = tokens_to_strs(cmd->tokens);
	if (ft_strchr(cmd->tokens->key, '/'))
		return (do_actual_path_cmd(cmd, args, envp));
	if (find_env_node(env, "PATH"))
	{
		path = ft_split(find_env_node(env, "PATH")->value, ':');
		i = 0;
		while (path[i])
		{
			path[i] = ft_strjoin(path[i], "/");
			path[i] = ft_strjoin(path[i], args[0]);
			i++;
		}
		while (*path)
		{
			execve(*path, args, envp);
			if (errno != ENOENT)
				break ;
			path++;
		}
		if (errno != ENOENT)
		{
			error_msg(strerror(errno));
			return (126);
		}
	}
	error_msg("command not found");
	return (127);
}

int	do_cmd(t_env *env, t_cmd *cmd)
{
	pid_t	pid;
	char	**args;
	int		status;

	if (check_builtin_cmd(cmd->tokens))
	{
		args = tokens_to_strs(cmd->tokens);
		status = execute_builtin_cmd(env, cmd, args);
		ft_free_split(args);
		return (status);
	}
	else
	{
		pid = fork();
		if (pid == -1)
			exit(EXIT_FAILURE);
		else if (pid == 0)
			exit(do_cmd_child(env, cmd));
		waitpid(-1, &status, 0);
		if (WIFEXITED(status))
			return (WEXITSTATUS(status));
		return (WCOREFLAG | WTERMSIG(status));
	}
}

int	do_pipe_cmd(t_env *env, t_cmd *cmd)
{

	if (check_builtin_cmd(cmd->tokens))
		return (execute_builtin_cmd(env, cmd, tokens_to_strs(cmd->tokens)));
	if (ft_strchr(cmd->tokens->key, '/'))
		return (do_actual_path_cmd(cmd, tokens_to_strs(cmd->tokens), get_env_list(&env)));
	return (do_cmd_child(env, cmd));
}

int	do_final_pipe_cmd(t_env *env, t_cmd *cmd, int n_pipe, int prev_fd)
{
	pid_t	pid;
	int		status;
	int		i;

	pid = fork();
	if (pid == -1)
	{
		kill(0, SIGKILL);
		exit(EXIT_FAILURE);
	}
	else if (pid == 0)
	{
		dup2(prev_fd, STDIN_FILENO);
		close(prev_fd);
		apply_redir(env, cmd);
		exit(do_pipe_cmd(env, cmd));
	}
	close(prev_fd);
	i = 0;
	while (i < n_pipe + 1)
	{
		if (waitpid(-1, &status, 0) == pid)
			g_status = status;
		i++;
	}
	return (g_status);
}

int	do_pipe(t_env *env, t_cmd *cmd, int n_pipe)
{
	int		i;
	pid_t	pid;
	int		prev_fd;
	int		pipe_fd[2];

	prev_fd = -1;
	i = 0;
	while (i < n_pipe)
	{
		if (pipe(pipe_fd))
		{
			kill(0, SIGKILL);
			exit(EXIT_FAILURE);
		}
		pid = fork();
		if (pid == -1)
		{
			kill(0, SIGKILL);
			exit(EXIT_FAILURE);
		}
		else if (pid == 0)
		{
			if (prev_fd != -1)
			{
				dup2(prev_fd, STDIN_FILENO);
				close(prev_fd);
			}
			close(pipe_fd[0]);
			dup2(pipe_fd[1], STDOUT_FILENO);
			close(pipe_fd[1]);
			apply_redir(env, cmd);
			exit(do_pipe_cmd(env, cmd));
		}
		if (prev_fd != -1)
			close(prev_fd);
		close(pipe_fd[1]);
		prev_fd = pipe_fd[0];
		cmd = cmd->next;
		i++;
	}
	return (do_final_pipe_cmd(env, cmd, n_pipe, prev_fd));
}

int	do_exec_function(t_env *env, t_token *tokens)
{
	t_cmd	*cmd;
	t_cmd	*tmp;
	int		n_cmd;

	cmd = make_cmd_list(env, tokens);
	set_redir(cmd);
	tmp = cmd;
	n_cmd = 0;
	while (tmp)
	{
		n_cmd++;
		tmp = tmp->next;
	}
	if (n_cmd == 1)
		g_status = do_cmd(env, cmd);
	else
		g_status = do_pipe(env, cmd, n_cmd - 1);
	del_cmd_list(cmd);
	return (g_status);
}

int main(void)
{
	t_token	*tokens;
	t_set	set;
	t_env	*env;
	char	*input;

	init_set(&set, &env);
	while (1)
	{
		signal(SIGINT, &sig_readline);
		signal(SIGQUIT, SIG_IGN);
		tcsetattr(STDOUT_FILENO, TCSANOW, &set.new_term);
		input = readline("minishell$ ");
		if (input == NULL)
		{
			write(1, "\e[Aminishell$ exit\n", 20);
			tcsetattr(STDOUT_FILENO, TCSANOW, &set.org_term);
			exit(g_status);
		}
		tcsetattr(STDOUT_FILENO, TCSANOW, &set.org_term);
		tokens = parse_input(input, env);
		if (!tokens)
		{
			input = ft_free(input);
			continue ;
		}
		g_status = do_exec_function(env, tokens);
		input = ft_free(input);
	}
	return (0);
}
