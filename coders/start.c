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
		if (init_coder(&coders[i], codexion, i + 1))
			return (1);
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

static void	join_and_cleanup(t_codexion *codexion, t_coder *coders,
		pthread_t *threads, pthread_t monitor_thread)
{
	long	i;

	i = -1;
	while (++i < codexion->coders_count)
		pthread_join(threads[i], NULL);
	pthread_join(monitor_thread, NULL);
	i = -1;
	while (++i < codexion->coders_count)
		pthread_mutex_destroy(&coders[i].dongle_right.mutex);
}

int	run_simulation(t_codexion *codexion)
{
	t_coder		*coders;
	pthread_t	*threads;
	pthread_t	monitor_thread;
	t_monitor	mon;
	long		i;

	coders = malloc(codexion->coders_count * sizeof(t_coder));
	if (!coders)
		return (1);
	threads = malloc(codexion->coders_count * sizeof(pthread_t));
	if (!threads)
		return (free(coders), 1);
	if (init_simulation(codexion, coders, &mon)
		|| pthread_mutex_init(&mon.mutex, NULL))
		return (free(coders), free(threads), 1);
	i = -1;
	while (++i < codexion->coders_count)
		init_coder_runtime(&coders[i], &mon);
	pthread_create(&monitor_thread, NULL, &monitor, &mon);
	i = -1;
	while (++i < codexion->coders_count)
		pthread_create(&threads[i], NULL, &routine, &coders[i]);
	join_and_cleanup(codexion, coders, threads, monitor_thread);
	pthread_mutex_destroy(&mon.mutex);
	free(coders);
	free(threads);
	return (0);
}
