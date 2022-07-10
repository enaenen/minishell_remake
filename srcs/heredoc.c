/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wchae <wchae@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/27 23:19:21 by wchae             #+#    #+#             */
/*   Updated: 2022/07/10 22:50:48 by wchae            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	heredoc_child(char *limiter, int *fd)
{
	t_buffer	*buf;
	char		*line;

	signal(SIGINT, SIG_DFL);
	close(fd[0]);
	buf = create_buf();
	while (TRUE)
	{
		line = readline("> ");
		if (!line)
			break ;
		if (ft_strncmp(line, limiter, -1) == 0)
		{
			free(line);
			break ;
		}
		add_str(buf, line);
		add_char(buf, '\n');
		free(line);
	}
	line = put_str(buf);
	write(fd[1], line, ft_strlen(line));
	close(fd[1]);
	del_buf(buf);
	exit(EXIT_SUCCESS);
}

char	*here_doc_parent(int fd)
{
	t_buffer	*buf;
	char		*str;

	buf = create_buf();
	while (TRUE)
	{
		str = get_next_line(fd);
		if (!str)
			break ;
		add_str(buf, str);
		free(str);
	}
	str = put_str(buf);
	del_buf(buf);
	return (str);
}

static char	*ft_heredoc(char *limiter)
{
	int			fd[2];
	int			status;
	char		*str;
	pid_t		pid;

	if (pipe(fd) == -1)
		return (NULL);
	pid = fork();
	if (pid == -1)
		return (NULL);
	if (pid == 0)
		heredoc_child(limiter, fd);
	close(fd[1]);
	waitpid(pid, &status, 0);
	if (status)
		return (NULL);
	str = here_doc_parent(fd[0]);
	close(fd[0]);
	return (str);
}

int	process_heredoc(t_token *token)
{
	char	*str;

	signal(SIGINT, &sig_here_doc);
	while (token)
	{
		if (ft_strncmp(token->key, "<<", 3) == 0)
		{
			token->next->value = rm_quote(token->next->key);
			str = ft_heredoc(token->next->value);
			if (str == NULL)
				return (EXIT_FAILURE);
			token->value = str;
			token = token->next;
		}
		token = token->next;
	}
	return (EXIT_SUCCESS);
}
