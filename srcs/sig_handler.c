/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sig_handler.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wchae <wchae@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/12 15:13:14 by wchae             #+#    #+#             */
/*   Updated: 2022/07/07 02:35:19 by wchae            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_replace_line(int var_stat)
{
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
	g_status = var_stat;
}

void	sig_readline(int sig)
{

	if (sig == SIGINT)
	{
		g_status = 1;
		printf("\n");
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}

	// if (sig == SIGINT)
	// 	ft_replace_line(1);
	// else if (sig == SIGQUIT)
	// {
	// 	rl_on_new_line();
	// 	rl_redisplay();
	// }
}
/**
void	ft_sig_handler(int status)
{
	pid_t	pid;

	pid = waitpid(-1, 0, WNOHANG);
	if (status == SIGINT)
	{
		if (pid == -1)
		{
			write(1, "\n", 1);
			ft_replace_line(1);
		}
		else
		{
			write(1, "\n", 1);
			g_status = 130;
		}
	}
	// else if (status == SIGQUIT && pid != -1)
	// {
	// 	write(1, "Quit: 3\n", 8);
	// 	g_status = 131;
	// }
	else
		ft_replace_line(131);
}
**/

void	sig_here_doc(int sig)
{
	if (sig == SIGINT)
		printf("\n");
}

// void	sig_here_doc_child(int sig)
// {
// 	if (sig == SIGQUIT)
// 	{
// 		rl_replace_line("", 0);
// 		rl_on_new_line();
// 		rl_redisplay();
// 	}
// }

void	sig_exec(int sig)
{
	if (sig == SIGINT)
		printf("^C\n");
	else if (sig == SIGQUIT)
		printf("Quit: 3\n");
}