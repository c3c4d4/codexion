/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wagyu <wagyu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/31 12:00:00 by wagyu          #+#    #+#             */
/*   Updated: 2026/05/31 12:00:00 by wagyu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

long long	now_ms(void)
{
	struct timeval	value;

	gettimeofday(&value, NULL);
	return ((long long)value.tv_sec * 1000LL + value.tv_usec / 1000LL);
}

void	to_timespec(long long timestamp, struct timespec *value)
{
	value->tv_sec = timestamp / 1000LL;
	value->tv_nsec = timestamp % 1000LL * 1000000LL;
}

int	wait_until(t_sim *sim, long long timestamp)
{
	struct timespec	limit;

	to_timespec(timestamp, &limit);
	while (!sim->stopped && now_ms() < timestamp)
		pthread_cond_timedwait(&sim->changed, &sim->lock, &limit);
	return (!sim->stopped);
}

int	wait_for_stop(t_sim *sim)
{
	while (!sim->stopped)
		pthread_cond_wait(&sim->changed, &sim->lock);
	return (0);
}
