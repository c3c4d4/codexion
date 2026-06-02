/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wagyu <wagyu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/31 12:00:00 by wagyu          #+#    #+#             */
/*   Updated: 2026/05/31 12:00:00 by wagyu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	run_phase(t_coder *coder, long long duration, char *message)
{
	t_sim	*sim;

	sim = coder->sim;
	if (sim->stopped || burnout_due(sim))
		return (0);
	log_action(sim, coder->id, message);
	return (wait_until(sim, now_ms() + duration));
}

static int	finish_compile(t_coder *coder)
{
	t_sim	*sim;

	sim = coder->sim;
	release_dongles(coder);
	if (burnout_due(sim))
		return (0);
	coder->compile_count++;
	if (coder->compile_count == sim->required)
		sim->completed++;
	if (sim->completed == sim->coder_count)
	{
		stop_simulation(sim);
		return (0);
	}
	return (1);
}

static int	compile_once(t_coder *coder)
{
	t_sim	*sim;

	sim = coder->sim;
	if (!acquire_dongles(coder))
		return (0);
	if (!run_phase(coder, sim->compile, "is compiling"))
	{
		release_dongles(coder);
		return (0);
	}
	return (finish_compile(coder));
}

void	*coder_routine(void *data)
{
	t_coder	*coder;
	t_sim	*sim;

	coder = (t_coder *)data;
	sim = coder->sim;
	pthread_mutex_lock(&sim->lock);
	coder->last_compile = sim->start;
	while (!sim->stopped)
	{
		if (!compile_once(coder))
			break ;
		if (!run_phase(coder, sim->debug, "is debugging"))
			break ;
		if (!run_phase(coder, sim->refactor, "is refactoring"))
			break ;
	}
	pthread_mutex_unlock(&sim->lock);
	return (NULL);
}
