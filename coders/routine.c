/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emarette <emarette@student.42lehavre.fr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/01 04:52:08 by emarette          #+#    #+#             */
/*   Updated: 2026/03/02 07:24:21 by emarette         ###   ########.fr       */
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

void	do_compile(t_coder *coder, t_dongle *first, t_dongle *second)
{
	if (take_dongle(coder, first))
	{
		if (take_dongle(coder, second))
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
			dongle_toggle_available(second);
		}
		dongle_toggle_available(first);
	}
}

void	compile(t_coder *coder)
{
	t_dongle	*first;
	t_dongle	*second;

	if (coder->id == (unsigned int)coder->codexion.coders_count)
	{
		first = coder->dongle_left;
		second = &coder->dongle_right;
	}
	else
	{
		first = &coder->dongle_right;
		second = coder->dongle_left;
	}
	do_compile(coder, first, second);
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