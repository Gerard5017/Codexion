/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsellier <lsellier@student.42lehavre.fr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/01 04:52:08 by emarette          #+#    #+#             */
/*   Updated: 2026/03/02 04:13:49 by lsellier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	refactor(t_coder *coder)
{
	if (!is_end(coder))
	{
		print_state(coder, REFACTOR_MSG);
		coder_wait(coder, coder->codexion.time_to_refactor);
	}
}

void	compile(t_coder *coder)
{
	if (take_dongle(coder, &coder->dongle_right))
	{
		if (take_dongle(coder, coder->dongle_left))
		{
			if (!is_end(coder))
			{
				print_state(coder, COMPILE_MSG);
				pthread_mutex_lock(&coder->monitor->mutex);
				coder->last_compile = get_time();
				coder->codexion.compile_rest--;
				coder->monitor->compile_rest--;
				if (coder->monitor->compile_rest <= 0)
				{
					coder->monitor->end = 1;
					coder->end = 1;
				}
				pthread_mutex_unlock(&coder->monitor->mutex);
				coder_wait(coder, coder->codexion.time_to_compile);
			}
			dongle_toggle_available(coder->dongle_left);
		}
		dongle_toggle_available(&coder->dongle_right);
	}
}

void	debug(t_coder *coder)
{
	if (!is_end(coder))
	{
		print_state(coder, DEBUG_MSG);
		coder_wait(coder, coder->codexion.time_to_debug);
	}
}

void	*routine(void *ptr)
{
	t_coder	*coder;

	coder = ptr;
	while (!is_end(coder) && coder->codexion.compile_rest > 0)
	{
		compile(coder);
		debug(coder);
		refactor(coder);
	}
	return (NULL);
}
