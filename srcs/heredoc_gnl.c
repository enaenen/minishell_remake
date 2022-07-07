/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_gnl.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wchae <wchae@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/27 23:19:21 by wchae             #+#    #+#             */
/*   Updated: 2022/07/07 21:12:16 by wchae            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	get_one_line(char *ptr, char **mem, char **ret)
{
	char		*tmp;

	if (ptr)
	{
		*ret = ft_strndup(*mem, ptr - *mem + 1);
		tmp = ft_strndup(ptr + 1, ft_strlen(ptr + 1));
		if (!*ret || !tmp)
			return (ERROR);
		free (*mem);
		*mem = tmp;
		return (SUCCESS);
	}
	else
	{
		if (*mem && **mem == '\0')
		{
			*ret = NULL;
			free (*mem);
		}
		else
			*ret = *mem;
		*mem = NULL;
		return (END_OF_FILE);
	}
}

static int	read_fd(int fd, char **mem, char **buf, char **ret)
{
	char		*ptr;
	char		*tmp;
	ssize_t		read_byte;

	while (1)
	{
		ptr = ft_strchr(*mem, '\n');
		if (ptr)
			break ;
		read_byte = read(fd, *buf, BUFFER_SIZE);
		if (read_byte == -1)
			return (ERROR);
		if (read_byte == 0)
			break ;
		(*buf)[read_byte] = '\0';
		tmp = ft_strjoin(*mem, *buf);
		if (!tmp)
			return (ERROR);
		free(*mem);
		*mem = tmp;
	}
	return (get_one_line(ptr, mem, ret));
}

char	*get_next_line(int fd)
{
	int			status;
	char		*buf;
	char		*ret;
	static char	*mem[OPEN_MAX + 1];

	if (fd < 0 || OPEN_MAX < fd || BUFFER_SIZE <= 0)
		return (NULL);
	if (!mem[fd])
		mem[fd] = ft_strndup("", 0);
	buf = malloc((size_t)BUFFER_SIZE + 1);
	if (!mem[fd] || !buf)
		return (NULL);
	ret = NULL;
	status = read_fd(fd, &mem[fd], &buf, &ret);
	free(buf);
	if (status == ERROR || status == END_OF_FILE)
	{
		free(mem[fd]);
		mem[fd] = NULL;
	}
	return (ret);
}
