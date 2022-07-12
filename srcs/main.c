/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wchae <wchae@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/11 16:28:59 by wchae             #+#    #+#             */
/*   Updated: 2022/07/12 14:12:25 by wchae            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char		*read_input(t_set *set);
static t_token	*parse_input(char *input);
static int		check_token(t_token	*tokens);
static int		check_quote_from_tokens(char *str);

int	main(void)
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
		input = read_input(&set);
		tokens = parse_input(input);
		if (!tokens)
			input = ft_free(input);
		else
		{
			tcsetattr(STDOUT_FILENO, TCSANOW, &set.org_term);
			signal(SIGINT, &sig_exec);
			signal(SIGQUIT, &sig_exec);
			g_status = do_exec_function(env, tokens);
			input = ft_free(input);
		}
	}
	return (0);
}

static char	*read_input(t_set *set)
{
	char	*input;

	input = readline("minishell$ ");
	if (input == NULL)
	{
		write(1, "\e[Aminishell$ exit\n", 20);
		tcsetattr(STDOUT_FILENO, TCSANOW, &set->org_term);
		exit(g_status);
	}
	return (input);
}

static t_token	*parse_input(char *input)
{
	t_token	*tokens;
	int		status;

	tokens = NULL;
	if (input[0])
		add_history(input);
	if (split_token(input, &tokens) == TRUE)
	{
		status = check_token(tokens);
		if (status)
		{
			del_env_lst(tokens);
			g_status = status;
			return (NULL);
		}
		status = process_heredoc(tokens);
		if (status)
		{
			del_env_lst(tokens);
			g_status = status;
			return (NULL);
		}
	}
	return (tokens);
}

static int	check_token(t_token	*tokens)
{
	while (tokens)
	{
		if (tokens->key[0] == '|'
			&& (!tokens->next || tokens->next->key[0] == '|'))
			return (error_msg("|"));
		else if (tokens->key[0] == '<' || tokens->key[0] == '>')
		{
			if (tokens->key[1] && tokens->key[0] != tokens->key[1])
				return (error_msg(&tokens->key[1]));
			else if (2 < ft_strlen(tokens->key))
				return (error_msg(&tokens->key[2]));
			else if (!tokens->next)
				return (error_msg(NULL));
			else if (tokens->next->key[0] == '<'
				|| tokens->next->key[0] == '>' || tokens->next->key[0] == '|')
				return (error_msg(tokens->next->key));
		}
		if (error_msg_quote(check_quote_from_tokens(tokens->key)))
			return (EXIT_FAILURE);
		tokens = tokens->next;
	}
	return (EXIT_SUCCESS);
}

static int	check_quote_from_tokens(char *str)
{
	int	q_flag;

	q_flag = 0;
	while (str && *str)
	{
		if (!(q_flag & D_QUOTE) && is_quote(*str) == S_QUOTE)
			q_flag ^= S_QUOTE;
		else if (!(q_flag & S_QUOTE) && is_quote(*str) == D_QUOTE)
			q_flag ^= D_QUOTE;
		str++;
	}
	return (q_flag);
}
