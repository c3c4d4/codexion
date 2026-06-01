/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   log.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wagyu <wagyu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/31 12:00:00 by wagyu          #+#    #+#             */
/*   Updated: 2026/05/31 12:00:00 by wagyu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	append_number(char *line, int index, long long number)
{
	char	digits[20];
	int		count;

	count = 0;
	if (!number)
		line[index++] = '0';
	while (number)
	{
		digits[count++] = number % 10 + '0';
		number /= 10;
	}
	while (count)
		line[index++] = digits[--count];
	return (index);
}

static int	append_text(char *line, int index, char *text)
{
	while (*text)
		line[index++] = *text++;
	return (index);
}

void	log_action(t_sim *sim, int id, char *action)
{
	char	line[128];
	int		length;

	length = append_number(line, 0, now_ms() - sim->start);
	line[length++] = ' ';
	length = append_number(line, length, id);
	line[length++] = ' ';
	length = append_text(line, length, action);
	line[length++] = '\n';
	pthread_mutex_lock(&sim->output_lock);
	write(1, line, length);
	pthread_mutex_unlock(&sim->output_lock);
}

void	stop_simulation(t_sim *sim)
{
	sim->stopped = 1;
	pthread_cond_broadcast(&sim->changed);
}
