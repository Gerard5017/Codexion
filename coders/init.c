/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emarette <emarette@student.42lehavre.fr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/02 07:29:15 by emarette          #+#    #+#             */
/*   Updated: 2026/03/02 07:29:16 by emarette         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	get_arg(long *l, char *arg)
{
	char	*end;

	*l = ft_strtol(arg, &end);
	if (*end || errno == ERANGE || *l <= 0)
		return (1);
	return (0);
}

int	init_dongle(t_dongle *dongle, t_codexion *codexion)
{
	dongle->available = 1;
	dongle->dongle_cooldown = codexion->dongle_cooldown;
	dongle->release_time = 0;
	if (pthread_mutex_init(&dongle->mutex, NULL))
		return (1);
	return (0);
}

int	init_arg(t_codexion *codexion, int ac, char **av)
{
	if (ac != 8)
		return (1);
	memset(codexion, 0, sizeof(t_codexion));
	if (get_arg(&codexion->coders_count, av[0])
		|| codexion->coders_count > MAX_CODERS)
		return (1);
	if (get_arg(&codexion->time_to_burnout, av[1]))
		return (1);
	if (get_arg(&codexion->time_to_compile, av[2]))
		return (1);
	if (get_arg(&codexion->time_to_debug, av[3]))
		return (1);
	if (get_arg(&codexion->time_to_refactor, av[4]))
		return (1);
	if (get_arg(&codexion->compile_rest, av[5]))
		return (1);
	if (get_arg(&codexion->dongle_cooldown, av[6]))
		return (1);
	if (ft_strcmp(av[7], "fifo") == 0)
		codexion->isfifo = 1;
	else if (ft_strcmp(av[7], "edf") == 0)
		codexion->isfifo = 0;
	else
		return (1);
	return (0);
}

int	init_coder(t_coder *coder, t_codexion *codexion, unsigned int id)
{
	if (init_dongle(&coder->dongle_right, codexion))
		return (1);
	coder->dongle_left = NULL;
	coder->id = id;
	coder->start_time = 0;
	coder->last_compile = 0;
	coder->end = 0;
	coder->codexion = *codexion;
	coder->requested_dongle = NULL;
	coder->request_seq = 0;
	coder->request_deadline = LONG_MAX;
	return (0);
}
