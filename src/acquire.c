/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   acquire.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wagyu <wagyu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/31 12:00:00 by wagyu          #+#    #+#             */
/*   Updated: 2026/05/31 12:00:00 by wagyu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	can_take(t_dongle *dongle, t_request *request, long long now)
{
	return (!dongle->owner && dongle->ready_at <= now
		&& heap_peek(&dongle->queue) == request);
}

static int	try_take_two(t_coder *coder)
{
	t_sim		*sim;
	long long	now;
	int			can_acquire;

	sim = coder->sim;
	now = now_ms();
	can_acquire = 0;
	lock_dongles(coder);
	if (coder->last_compile + sim->burnout > now
		&& can_take(&sim->dongles[coder->left], &coder->left_request, now)
		&& can_take(&sim->dongles[coder->right], &coder->right_request, now))
		can_acquire = 1;
	if (can_acquire)
		claim_dongles(coder, now);
	unlock_dongles(coder);
	return (can_acquire);
}

static long long	next_attempt(t_coder *coder)
{
	t_sim		*sim;
	long long	next;

	sim = coder->sim;
	lock_dongles(coder);
	next = coder->last_compile + sim->burnout;
	if (sim->dongles[coder->left].ready_at > now_ms()
		&& sim->dongles[coder->left].ready_at < next)
		next = sim->dongles[coder->left].ready_at;
	if (sim->dongles[coder->right].ready_at > now_ms()
		&& sim->dongles[coder->right].ready_at < next)
		next = sim->dongles[coder->right].ready_at;
	unlock_dongles(coder);
	return (next);
}

static int	acquire_single(t_coder *coder)
{
	t_sim		*sim;
	t_dongle	*dongle;

	sim = coder->sim;
	dongle = &sim->dongles[0];
	pthread_mutex_lock(&dongle->lock);
	dongle->owner = coder->id;
	pthread_mutex_unlock(&dongle->lock);
	log_action(sim, coder->id, "has taken a dongle");
	wait_for_stop(sim);
	pthread_mutex_lock(&dongle->lock);
	dongle->owner = 0;
	pthread_mutex_unlock(&dongle->lock);
	return (0);
}

int	acquire_dongles(t_coder *coder)
{
	t_sim			*sim;
	struct timespec	limit;

	sim = coder->sim;
	if (sim->coder_count == 1)
		return (acquire_single(coder));
	enqueue_requests(coder);
	while (!sim->stopped)
	{
		if (burnout_due(sim))
			return (wait_for_stop(sim));
		if (try_take_two(coder))
		{
			log_action(sim, coder->id, "has taken a dongle");
			log_action(sim, coder->id, "has taken a dongle");
			return (1);
		}
		to_timespec(next_attempt(coder), &limit);
		pthread_cond_timedwait(&sim->changed, &sim->lock, &limit);
	}
	return (0);
}
