/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_error.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wchae <wchae@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/09 17:12:26 by wchae             #+#    #+#             */
/*   Updated: 2022/07/11 12:55:35 by wchae            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	error_msg(char *msg)
{
	write(2, "minishell: ", 12);
	if (!msg)
	{
		write(2, "syntax error near unexpected token 'newline'\n", 45);
		return (EXIT_FAILURE);
	}
	else if (msg[0] == '|' || msg[0] == '<' || msg[0] == '>')
	{
		write(2, "syntax error near unexpected token '", 36);
		write(2, msg, ft_strlen(msg));
		write(2, "'\n", 2);
		return (ERROR);
	}
	write(2, msg, ft_strlen(msg));
	write(2, ": ", 2);
	write(2, strerror(errno), ft_strlen(strerror(errno)));
	write(2, "\n", 1);
	if (errno == EACCES)
		return (126);
	return (127);
}

int	error_msg_cmd_not_found(char *msg)
{
	write(2, "minishell: ", 12);
	write(2, msg, ft_strlen(msg));
	write(2, ": ", 2);
	write(2, "command not found\n", 18);
	return (127);
}

int	error_msg_ambiguous(char *str)
{
	t_buffer	*buf;
	char		*err_msg;

	buf = create_buf();
	add_str(buf, "minishell: ");
	add_str(buf, str);
	add_str(buf, ": ambiguous redirect\n");
	err_msg = put_str(buf);
	ft_putstr_fd(err_msg, STDERR_FILENO);
	del_buf(buf);
	free(err_msg);
	return (EXIT_FAILURE);
}

int	fd_print_err(char *str)
{
	error_msg(str);
	return (EXIT_FAILURE);
}
