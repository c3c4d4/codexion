/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wagyu <wagyu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/31 12:00:00 by wagyu          #+#    #+#             */
/*   Updated: 2026/05/31 12:00:00 by wagyu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	init_base(t_sim *sim)
{
	if (pthread_mutex_init(&sim->lock, NULL))
		return (0);
	sim->base_initialized = 1;
	if (pthread_mutex_init(&sim->output_lock, NULL))
		return (0);
	sim->base_initialized = 2;
	if (pthread_cond_init(&sim->changed, NULL))
		return (0);
	sim->base_initialized = 3;
	return (1);
}

static int	init_dongles(t_sim *sim)
{
	t_dongle	*dongle;

	sim->dongles = malloc(sizeof(t_dongle) * sim->coder_count);
	if (!sim->dongles)
		return (0);
	memset(sim->dongles, 0, sizeof(t_dongle) * sim->coder_count);
	while (sim->dongles_initialized < sim->coder_count)
	{
		dongle = &sim->dongles[sim->dongles_initialized];
		dongle->queue.capacity = 2;
		dongle->queue.items = malloc(sizeof(t_request *) * 2);
		if (!dongle->queue.items || pthread_mutex_init(&dongle->lock, NULL))
			return (0);
		sim->dongles_initialized++;
	}
	return (1);
}

static void	init_coder(t_sim *sim, int index)
{
	t_coder	*coder;

	coder = &sim->coders[index];
	coder->sim = sim;
	coder->id = index + 1;
	coder->left = index;
	coder->right = (index + sim->coder_count - 1) % sim->coder_count;
	coder->left_request.coder = coder;
	coder->right_request.coder = coder;
}

static int	init_coders(t_sim *sim)
{
	int	index;

	sim->coders = malloc(sizeof(t_coder) * sim->coder_count);
	if (!sim->coders)
		return (0);
	memset(sim->coders, 0, sizeof(t_coder) * sim->coder_count);
	index = 0;
	while (index < sim->coder_count)
		init_coder(sim, index++);
	return (1);
}

int	init_sim(t_sim *sim)
{
	if (!init_base(sim) || !init_dongles(sim) || !init_coders(sim))
		return (0);
	return (1);
}
