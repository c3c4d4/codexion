/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   claim.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wagyu <wagyu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/31 12:00:00 by wagyu          #+#    #+#             */
/*   Updated: 2026/05/31 12:00:00 by wagyu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	claim_dongles(t_coder *coder, long long now)
{
	t_sim	*sim;

	sim = coder->sim;
	heap_pop(sim, &sim->dongles[coder->left].queue);
	heap_pop(sim, &sim->dongles[coder->right].queue);
	sim->dongles[coder->left].owner = coder->id;
	sim->dongles[coder->right].owner = coder->id;
	coder->last_compile = now;
	pthread_cond_broadcast(&sim->changed);
}
