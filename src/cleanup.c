/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wagyu <wagyu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/31 12:00:00 by wagyu          #+#    #+#             */
/*   Updated: 2026/05/31 12:00:00 by wagyu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	cleanup_dongles(t_sim *sim)
{
	int	index;

	index = 0;
	while (index < sim->dongles_initialized)
	{
		pthread_mutex_destroy(&sim->dongles[index].lock);
		free(sim->dongles[index].queue.items);
		index++;
	}
	if (sim->dongles && index < sim->coder_count)
		free(sim->dongles[index].queue.items);
	free(sim->dongles);
}

static void	cleanup_base(t_sim *sim)
{
	if (sim->base_initialized >= 3)
		pthread_cond_destroy(&sim->changed);
	if (sim->base_initialized >= 2)
		pthread_mutex_destroy(&sim->output_lock);
	if (sim->base_initialized >= 1)
		pthread_mutex_destroy(&sim->lock);
}

void	cleanup_sim(t_sim *sim)
{
	free(sim->coders);
	cleanup_dongles(sim);
	cleanup_base(sim);
}
