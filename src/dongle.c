/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wagyu <wagyu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/31 12:00:00 by wagyu          #+#    #+#             */
/*   Updated: 2026/05/31 12:00:00 by wagyu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	lock_dongles(t_coder *coder)
{
	t_sim	*sim;

	sim = coder->sim;
	if (coder->left < coder->right)
	{
		pthread_mutex_lock(&sim->dongles[coder->left].lock);
		pthread_mutex_lock(&sim->dongles[coder->right].lock);
	}
	else
	{
		pthread_mutex_lock(&sim->dongles[coder->right].lock);
		pthread_mutex_lock(&sim->dongles[coder->left].lock);
	}
}

void	unlock_dongles(t_coder *coder)
{
	t_sim	*sim;

	sim = coder->sim;
	pthread_mutex_unlock(&sim->dongles[coder->left].lock);
	pthread_mutex_unlock(&sim->dongles[coder->right].lock);
}

static void	fill_request(t_sim *sim, t_request *request, t_coder *coder)
{
	request->coder = coder;
	request->sequence = sim->sequence;
	request->deadline = coder->last_compile + sim->burnout;
}

void	enqueue_requests(t_coder *coder)
{
	t_sim	*sim;

	sim = coder->sim;
	sim->sequence++;
	fill_request(sim, &coder->left_request, coder);
	fill_request(sim, &coder->right_request, coder);
	lock_dongles(coder);
	heap_push(sim, &sim->dongles[coder->left].queue, &coder->left_request);
	heap_push(sim, &sim->dongles[coder->right].queue, &coder->right_request);
	unlock_dongles(coder);
	pthread_cond_broadcast(&sim->changed);
}

void	release_dongles(t_coder *coder)
{
	t_sim		*sim;
	long long	ready_at;

	sim = coder->sim;
	ready_at = now_ms() + sim->cooldown;
	lock_dongles(coder);
	sim->dongles[coder->left].owner = 0;
	sim->dongles[coder->left].ready_at = ready_at;
	sim->dongles[coder->right].owner = 0;
	sim->dongles[coder->right].ready_at = ready_at;
	unlock_dongles(coder);
	pthread_cond_broadcast(&sim->changed);
}
