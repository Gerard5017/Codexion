/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   start.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsellier <lsellier@student.42lehavre.fr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/01 04:24:12 by emarette          #+#    #+#             */
/*   Updated: 2026/03/02 04:05:41 by lsellier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	init_simulation(t_codexion *codexion, t_coder *coders, t_monitor *monitor)
{
	long	i;

	i = -1;
	while (++i < codexion->coders_count)
	{
		init_coder(&coders[i], codexion, i + 1);
		coders[i].dongle_left = &coders[(i + 1)
			% codexion->coders_count].dongle_right;
	}
	monitor->coders = coders;
	monitor->count = codexion->coders_count;
	monitor->compile_rest = codexion->compile_rest * codexion->coders_count;
	monitor->end = 0;
	return (0);
}

void	init_coder_runtime(t_coder *coder, t_monitor *monitor)
{
	coder->start_time = get_time();
	coder->last_compile = coder->start_time;
	coder->monitor = monitor;
}

int	run_simulation(t_codexion *codexion)
{
	t_coder		coders[MAX_CODERS];
	pthread_t	threads[MAX_CODERS];
	pthread_t	monitor_thread;
	t_monitor	mon;
	long		i;

	if (init_simulation(codexion, coders, &mon))
		return (1);
	if (pthread_mutex_init(&mon.mutex, NULL))
		return (1);
	i = -1;
	while (++i < codexion->coders_count)
		init_coder_runtime(&coders[i], &mon);
	pthread_create(&monitor_thread, NULL, &monitor, &mon);
	i = -1;
	while (++i < codexion->coders_count)
		pthread_create(&threads[i], NULL, &routine, &coders[i]);
	i = -1;
	while (++i < codexion->coders_count)
		pthread_join(threads[i], NULL);
	pthread_join(monitor_thread, NULL);
	pthread_mutex_destroy(&mon.mutex);
	return (0);
}
