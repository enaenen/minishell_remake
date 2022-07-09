/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_signal.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seseo <seseo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/12 15:13:14 by wchae             #+#    #+#             */
/*   Updated: 2022/07/10 03:02:11 by seseo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
}

void	sig_here_doc(int sig)
{
	if (sig == SIGINT)
		printf("\n");
}

void	sig_exec(int sig)
{
	if (sig == SIGINT)
		printf("\n");
	else if (sig == SIGQUIT)
		printf("Quit: 3\n");
}
