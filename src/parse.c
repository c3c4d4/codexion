/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wagyu <wagyu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/31 12:00:00 by wagyu          #+#    #+#             */
/*   Updated: 2026/05/31 12:00:00 by wagyu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	parse_number(char *value, long long *result)
{
	long long	number;
	int			index;

	if (!value || !value[0])
		return (0);
	number = 0;
	index = 0;
	while (value[index])
	{
		if (value[index] < '0' || value[index] > '9')
			return (0);
		if (number > (INT_MAX - (value[index] - '0')) / 10)
			return (0);
		number = number * 10 + value[index++] - '0';
	}
	*result = number;
	return (1);
}

static int	parse_values(t_sim *sim, char **argv)
{
	long long	count;

	if (!parse_number(argv[1], &count) || count > INT_MAX)
		return (0);
	sim->coder_count = (int)count;
	if (!parse_number(argv[2], &sim->burnout)
		|| !parse_number(argv[3], &sim->compile)
		|| !parse_number(argv[4], &sim->debug)
		|| !parse_number(argv[5], &sim->refactor)
		|| !parse_number(argv[7], &sim->cooldown))
		return (0);
	return (1);
}

static int	parse_required(t_sim *sim, char *value)
{
	long long	required;

	if (!parse_number(value, &required) || required > INT_MAX)
		return (0);
	sim->required = (int)required;
	return (1);
}

int	parse_args(t_sim *sim, int argc, char **argv)
{
	if (argc != 9 || !parse_values(sim, argv)
		|| !parse_required(sim, argv[6]))
		return (0);
	if (!strcmp(argv[8], "fifo"))
		sim->scheduler = FIFO;
	else if (!strcmp(argv[8], "edf"))
		sim->scheduler = EDF;
	else
		return (0);
	if (sim->coder_count <= 0 || sim->burnout <= 0 || sim->compile <= 0
		|| sim->debug <= 0 || sim->refactor <= 0 || sim->required <= 0)
		return (0);
	return (1);
}
