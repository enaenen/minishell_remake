/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seseo <seseo@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/27 23:19:21 by wchae             #+#    #+#             */
/*   Updated: 2022/07/07 21:05:09 by seseo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	heredoc_child(char *limiter, int *fd)
{
	t_buffer	*buf;
	char		*line;

	signal(SIGINT, SIG_DFL);
	close(fd[0]);
	buf = create_buf();
	while (TRUE)
	{
		line = readline(">");
		if (!line)
			break ;
		add_str(buf, line);
		add_char(buf, '\n');
		free(line);
	}
	line = put_str(buf);
	del_buf(buf);
	write(fd[1], line, ft_strlen(line));
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
}

static char	*ft_heredoc(char *limiter)
{
	int			fd[2];
	int			status;
	char		*str;
	pid_t		pid;

	if (pipe(fd) == -1)
		return (error_msg("pipe"));
	pid = fork();
	if (pid == -1)
		return (error_msg("fork"));
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

void	process_heredoc(t_list *token)
{
	int	org_stdin;

	signal(SIGINT, &sig_here_doc);
	while (token)
	{
		if (ft_strncmp(token->data, "<<", 3) == 0)
		{
			ft_heredoc(token->next->data);
			token = token->next;
		}
		token = token->next;
	}
}
