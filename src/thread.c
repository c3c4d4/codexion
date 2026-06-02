/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wagyu <wagyu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/31 12:00:00 by wagyu          #+#    #+#             */
/*   Updated: 2026/05/31 12:00:00 by wagyu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	join_threads(t_sim *sim, int count, int monitor_started)
{
	int	index;

	index = 0;
	while (index < count)
		pthread_join(sim->coders[index++].thread, NULL);
	if (monitor_started)
		pthread_join(sim->monitor, NULL);
}

static int	fail_start(t_sim *sim, int count, int monitor_started)
{
	pthread_mutex_lock(&sim->lock);
	stop_simulation(sim);
	pthread_mutex_unlock(&sim->lock);
	join_threads(sim, count, monitor_started);
	return (0);
}

int	start_threads(t_sim *sim)
{
	int	index;

	sim->start = now_ms();
	index = 0;
	while (index < sim->coder_count)
		sim->coders[index++].last_compile = sim->start;
	if (pthread_create(&sim->monitor, NULL, monitor_routine, sim))
		return (0);
	index = 0;
	while (index < sim->coder_count)
	{
		if (pthread_create(&sim->coders[index].thread, NULL,
				coder_routine, &sim->coders[index]))
			return (fail_start(sim, index, 1));
		index++;
	}
	join_threads(sim, sim->coder_count, 1);
	return (1);
}
