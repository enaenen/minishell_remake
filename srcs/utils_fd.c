/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_fd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wchae <wchae@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/10 22:54:41 by wchae             #+#    #+#             */
/*   Updated: 2022/07/10 22:55:00 by wchae            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	backup_fd(int backup_io[2])
{
	backup_io[0] = dup(STDIN_FILENO);
	backup_io[1] = dup(STDOUT_FILENO);
}

void	restore_fd(int backup_io[2])
{
	int	rd_io[2];

	rd_io[0] = dup(STDIN_FILENO);
	rd_io[1] = dup(STDOUT_FILENO);
	dup2(backup_io[0], STDIN_FILENO);
	dup2(backup_io[1], STDOUT_FILENO);
	close(backup_io[0]);
	close(backup_io[1]);
	close(rd_io[0]);
	close(rd_io[1]);
}
