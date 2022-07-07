/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wchae <wchae@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/27 23:19:21 by wchae             #+#    #+#             */
/*   Updated: 2022/07/07 03:49:07 by wchae            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static char	*ft_merge_str(char *line, char buf)
{
	int		size;
	char	*str;
	int		i;

	size = ft_strlen(line);
	str = (char *)malloc(sizeof(char) * (size + 2));
	if (!str)
		return (NULL);
	i = 0;
	while (line[i] != 0)
	{
		str[i] = line[i];
		i++;
	}
	free(line);
	str[i++] = buf;
	str[i] = '\0';
	return (str);
}

static int	get_next_line(char **line)
{
	char	buf;
	int		ret;

	*line = (char *)malloc(1);
	if (*line == NULL)
		return (-1);
	(*line)[0] = 0;
	ret = read(0, &buf, 1);
	while (buf != '\n' && buf != '\0')
	{
		*line = ft_merge_str(*line, buf);
		if (*line == 0)
			return (-1);
		ret = read(0, &buf, 1);
	}
	if (buf == '\n')
		return (1);
	return (0);
}

static void	print_line(char *line, char *limiter, int fd)
{
	if (ft_strncmp(line, limiter, ft_strlen(limiter)) == 0)
	{
		free(line);
		close(fd);
		exit(EXIT_SUCCESS);
	}
	write(1, "> ", 2);
	write(fd, line, ft_strlen(line));
	write(fd, "\n", 1);
}

static void	heredoc_child(char *limiter, int *fd)
{
	char	*line;

	signal(SIGINT, SIG_DFL);
	close(fd[0]);
	write(1, "> ", 2);
	while (get_next_line(&line))
		print_line(line, limiter, fd[1]);
	exit(EXIT_FAILURE);
}

static int	ft_heredoc(char *limiter)
{
	int		fd[2];
	int		status;
	pid_t	pid;
	
	if (pipe(fd) == -1)
		return (error_msg("pipe"));
	pid = fork();
	if (pid == -1)
		return (error_msg("fork"));
	if (pid == 0)
		heredoc_child(limiter, fd);
	close(fd[1]);
	waitpid(pid, &status, 0);
	dup2(fd[0], STDIN_FILENO);
	close(fd[0]);
	
	return (TRUE);
}

void	process_heredoc(t_list *token)
{
	int org_stdin;

	signal(SIGINT, &sig_here_doc);
	org_stdin = dup(STDIN_FILENO);
	while (token)
	{
		if (ft_strncmp(token->data, "<<", 3) == 0)
		{
			dup2(org_stdin, STDIN_FILENO);
			ft_heredoc(token->next->data);
			token = token->next;
		}
		token = token->next;
	}
}
