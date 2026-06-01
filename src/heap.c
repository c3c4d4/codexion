/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wagyu <wagyu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/31 12:00:00 by wagyu          #+#    #+#             */
/*   Updated: 2026/05/31 12:00:00 by wagyu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	request_before(t_sim *sim, t_request *one, t_request *two)
{
	if (sim->scheduler == EDF && one->deadline != two->deadline)
		return (one->deadline < two->deadline);
	if (one->sequence != two->sequence)
		return (one->sequence < two->sequence);
	return (one->coder->id < two->coder->id);
}

static void	swap_requests(t_request **one, t_request **two)
{
	t_request	*temporary;

	temporary = *one;
	*one = *two;
	*two = temporary;
}

int	heap_push(t_sim *sim, t_heap *heap, t_request *request)
{
	int	index;
	int	parent;

	if (heap->size >= heap->capacity)
		return (0);
	index = heap->size++;
	heap->items[index] = request;
	while (index > 0)
	{
		parent = (index - 1) / 2;
		if (!request_before(sim, heap->items[index], heap->items[parent]))
			break ;
		swap_requests(&heap->items[index], &heap->items[parent]);
		index = parent;
	}
	return (1);
}

t_request	*heap_pop(t_sim *sim, t_heap *heap)
{
	t_request	*result;
	int			index;
	int			child;

	if (!heap->size)
		return (NULL);
	result = heap->items[0];
	heap->items[0] = heap->items[--heap->size];
	index = 0;
	while (index * 2 + 1 < heap->size)
	{
		child = index * 2 + 1;
		if (child + 1 < heap->size && request_before(sim,
				heap->items[child + 1], heap->items[child]))
			child++;
		if (!request_before(sim, heap->items[child], heap->items[index]))
			break ;
		swap_requests(&heap->items[index], &heap->items[child]);
		index = child;
	}
	return (result);
}

t_request	*heap_peek(t_heap *heap)
{
	if (!heap->size)
		return (NULL);
	return (heap->items[0]);
}
