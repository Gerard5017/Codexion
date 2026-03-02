/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsellier <lsellier@student.42lehavre.fr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/02 01:53:43 by lsellier          #+#    #+#             */
/*   Updated: 2026/03/02 01:53:43 by lsellier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	*monitor(void *ptr)
{
	t_monitor	*mon;
	long		i;

	mon = ptr;
	while (1)
	{
		pthread_mutex_lock(&mon->mutex);
		if (mon->compile_rest <= 0 || mon->end)
		{
			mon->end = 1;
			i = -1;
			while (++i < mon->count)
				mon->coders[i].end = 1;
			pthread_mutex_unlock(&mon->mutex);
			return (NULL);
		}
		pthread_mutex_unlock(&mon->mutex);
		i = -1;
		while (++i < mon->count)
			if (is_end(&mon->coders[i]))
				return (NULL);
		usleep(1000 * 10);
	}
}
