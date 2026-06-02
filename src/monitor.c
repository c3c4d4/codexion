/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wagyu <wagyu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/31 12:00:00 by wagyu          #+#    #+#             */
/*   Updated: 2026/05/31 12:00:00 by wagyu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static t_coder	*find_burnout(t_sim *sim, long long now)
{
	t_coder	*burned_out;
	int		index;

	burned_out = NULL;
	index = 0;
	while (index < sim->coder_count)
	{
		if (sim->coders[index].last_compile + sim->burnout <= now
			&& (!burned_out || sim->coders[index].last_compile
				< burned_out->last_compile))
			burned_out = &sim->coders[index];
		index++;
	}
	return (burned_out);
}

int	burnout_due(t_sim *sim)
{
	return (find_burnout(sim, now_ms()) != NULL);
}

static long long	next_deadline(t_sim *sim)
{
	long long	deadline;
	long long	candidate;
	int			index;

	index = 0;
	deadline = sim->coders[0].last_compile + sim->burnout;
	while (++index < sim->coder_count)
	{
		candidate = sim->coders[index].last_compile + sim->burnout;
		if (candidate < deadline)
			deadline = candidate;
	}
	return (deadline);
}

void	*monitor_routine(void *data)
{
	t_sim			*sim;
	t_coder			*burned_out;
	struct timespec	limit;

	sim = (t_sim *)data;
	pthread_mutex_lock(&sim->lock);
	while (!sim->stopped)
	{
		burned_out = find_burnout(sim, now_ms());
		if (burned_out)
		{
			log_action(sim, burned_out->id, "burned out");
			stop_simulation(sim);
			break ;
		}
		to_timespec(next_deadline(sim), &limit);
		pthread_cond_timedwait(&sim->changed, &sim->lock, &limit);
	}
	pthread_mutex_unlock(&sim->lock);
	return (NULL);
}
