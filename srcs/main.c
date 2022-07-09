/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wchae <wchae@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/11 16:28:59 by wchae             #+#    #+#             */
/*   Updated: 2022/07/09 19:18:03 by wchae            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_token(t_token	*token)
{
	int i;

	i = 0;
	while (token)
	{
		if (token->key[0] == '|' && (i == 0
				|| !token->next || token->next->key[0] == '|'))
			return (error_msg("|"));
		else if (token->key[0] == '<' || token->key[0] == '>')
		{
			if (token->key[1] && token->key[0] != token->key[1])
				return (error_msg(&token->key[1]));
			else if (2 < ft_strlen(token->key))
				return (error_msg(&token->key[2]));
			else if (!token->next)
				return (error_msg(NULL));
			else if (token->next->key[0] == '<'
				|| token->next->key[0] == '>' || token->next->key[0] == '|')
				return (error_msg(token->next->key));
		}
		i++;
		token = token->next;
	}
	return (TRUE);
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
}

void	backup_fd(int backup_io[2])
{
	backup_io[0] = dup(STDIN_FILENO);
	backup_io[1] = dup(STDOUT_FILENO);
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
