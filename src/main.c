/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wagyu <wagyu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/31 12:00:00 by wagyu          #+#    #+#             */
/*   Updated: 2026/05/31 12:00:00 by wagyu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	print_error(char *message)
{
	fprintf(stderr, "Error: %s\n", message);
	return (1);
}

int	main(int argc, char **argv)
{
	t_sim	sim;

	memset(&sim, 0, sizeof(sim));
	if (!parse_args(&sim, argc, argv))
		return (print_error("invalid arguments"));
	if (!init_sim(&sim))
	{
		cleanup_sim(&sim);
		return (print_error("initialization failed"));
	}
	if (!start_threads(&sim))
	{
		cleanup_sim(&sim);
		return (print_error("thread creation failed"));
	}
	cleanup_sim(&sim);
	return (0);
}
